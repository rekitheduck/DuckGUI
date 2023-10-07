#include "Window.h"

#include <iostream>

#include "wayland-client.h"

using namespace DuckGUI;

static void registry_handle_global(void* data, wl_registry* registry, uint32_t name, const char* interface,
                                   uint32_t version) {
    static_cast<Window*>(data)->handleGlobal(name, std::string(interface), version);
}

static void registry_handle_global_remove(void* data, wl_registry* registry, uint32_t name) {
    // This space deliberately left blank
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
    return state;
}

ReturnStatus Window::initDisplay() {
    m_display = wl_display_connect(nullptr);
    if (m_display == nullptr) {
        std::cout << "connection f ailed :(" << std::endl;
        return ReturnStatus::Bad;
    }

    // temp
    // wl_display_disconnect(m_display);
    // std::cout << "connection good :)" << std::endl;
    // m_display = nullptr;

    return ReturnStatus::Good;
}

ReturnStatus Window::initRegistry() {
    m_registry = wl_display_get_registry(m_display);

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
    }
}
