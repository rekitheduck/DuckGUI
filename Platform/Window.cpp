#include "Window.h"

#include <iostream>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include <wayland-client.h>

#include "xdg-shell-client-protocol.h"

using namespace DuckGUI;

static void registry_handle_global(void* data, wl_registry* registry, uint32_t name, const char* interface,
                                   uint32_t version) {
    static_cast<Window*>(data)->handleGlobal(name, std::string(interface), version);
}

static void registry_handle_global_remove(void* data, wl_registry* registry, uint32_t name) {
    // This space deliberately left blank
}

static void xdg_wm_base_ping(void* data, xdg_wm_base* xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static void randname(char* buf) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A' + (r & 15) + (r & 16) * 2;
        r >>= 5;
    }
}

static int create_shm_file() {
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        randname(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

static int allocate_shm_file(size_t size) {
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static void wl_buffer_release(void* data, struct wl_buffer* wl_buffer) {
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static wl_buffer* draw_frame(Window* window) {
    const int width = 200;
    const int height = 150;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t* data = static_cast<uint32_t*>(mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool* pool = wl_shm_create_pool(window->getShmPtr(), fd, size);
    struct wl_buffer* buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    // Draw checkerboxed background
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8)
                data[y * width + x] = 0xFF666666;
            else
                data[y * width + x] = 0xFFEEEEEE;
        }
    }

    munmap(data, size);

    const wl_buffer_listener wl_buffer_listener = {
        .release = wl_buffer_release,
    };
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static void xdg_surface_configure(void* data, xdg_surface* xdg_surface, uint32_t serial) {
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_buffer* buffer = draw_frame(static_cast<Window*>(data));
    wl_surface_attach(static_cast<Window*>(data)->getSurfacePtr(), buffer, 0, 0);
    wl_surface_commit(static_cast<Window*>(data)->getSurfacePtr());
}

ReturnStatus Window::initWindow() {
    auto state = initDisplay();
    if (state == ReturnStatus::Bad) {
        return state;
    }

    state = initRegistry();
    if (state == ReturnStatus::Bad) {
        return state;
    }

    m_surface = wl_compositor_create_surface(m_compositor);
    m_xdg_surface = xdg_wm_base_get_xdg_surface(m_xdg_wm_base, m_surface);

    const xdg_surface_listener surface_listener = {.configure = xdg_surface_configure};

    xdg_surface_add_listener(m_xdg_surface, &surface_listener, this);

    m_xdg_toplevel = xdg_surface_get_toplevel(m_xdg_surface);
    xdg_toplevel_set_title(m_xdg_toplevel, "Test Window");
    wl_surface_commit(m_surface);

    while (wl_display_dispatch(m_display)) {
        /* This space deliberately left blank */
    }

    return state;
}

ReturnStatus Window::initDisplay() {
    m_display = wl_display_connect(nullptr);
    if (m_display == nullptr) {
        std::cout << "connection f ailed :(" << std::endl;
        return ReturnStatus::Bad;
    }

    return ReturnStatus::Good;
}

ReturnStatus Window::initRegistry() {
    m_registry = wl_display_get_registry(m_display);
    if (m_registry == nullptr) {
        std::cout << "failed to get registry" << std::endl;
        return ReturnStatus::Bad;
    }

    const wl_registry_listener registry_listener = {
        .global = registry_handle_global,
        .global_remove = registry_handle_global_remove,
    };

    wl_registry_add_listener(m_registry, &registry_listener, this);
    wl_display_roundtrip(m_display);

    return ReturnStatus::Good;
}

void Window::handleGlobal(uint32_t id, std::string interface_name, uint32_t version) {
    // We need to set up the compositor
    if (interface_name == wl_compositor_interface.name) {
        m_compositor = static_cast<wl_compositor*>(wl_registry_bind(m_registry, id, &wl_compositor_interface, 4));
    } else if (interface_name == wl_shm_interface.name) {
        m_shm = static_cast<wl_shm*>(wl_registry_bind(m_registry, id, &wl_shm_interface, 1));
    } else if (interface_name == xdg_wm_base_interface.name) {
        m_xdg_wm_base = static_cast<xdg_wm_base*>(wl_registry_bind(m_registry, id, &xdg_wm_base_interface, 1));
        const xdg_wm_base_listener listener = {
            .ping = xdg_wm_base_ping,
        };
        xdg_wm_base_add_listener(m_xdg_wm_base, &listener, this);
    }
}
