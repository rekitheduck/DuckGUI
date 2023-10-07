#pragma once

struct wl_display;

namespace DuckGUI {

enum class ReturnStatus { Good, Bad };

class Window {
   public:
    ReturnStatus initDisplay();

   private:
    wl_display* m_display{nullptr};
};
} // namespace DuckGUI
