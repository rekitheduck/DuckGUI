#pragma once

#include <string>

struct wl_display;
struct wl_registry;
struct wl_compositor;
struct wl_surface;
struct wl_shm;
struct xdg_surface;
struct xdg_toplevel;
struct xdg_wm_base;

namespace DuckGUI {

enum class ReturnStatus { Good, Bad };

class Window {
   public:
    ReturnStatus initWindow();
    // TODO: private these?
    ReturnStatus initDisplay();
    ReturnStatus initRegistry();
    ReturnStatus initCompositor();

    void handleGlobal(uint32_t id, std::string interface_name, uint32_t version);
    wl_surface* getSurfacePtr() { return m_surface; }
    wl_shm* getShmPtr() { return m_shm; }

   private:
    wl_display* m_display{nullptr};
    wl_registry* m_registry{nullptr};
    wl_compositor* m_compositor{nullptr};
    wl_surface* m_surface{nullptr};
    wl_shm* m_shm{nullptr};
    xdg_surface* m_xdg_surface{nullptr};
    xdg_toplevel* m_xdg_toplevel{nullptr};
    xdg_wm_base* m_xdg_wm_base{nullptr};
};
} // namespace DuckGUI
