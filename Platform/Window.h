#pragma once

#include <string>

struct wl_display;
struct wl_registry;
struct wl_compositor;

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

   private:
    wl_display* m_display{nullptr};
    wl_registry* m_registry{nullptr};
    wl_compositor* m_compositor{nullptr};
};
} // namespace DuckGUI
