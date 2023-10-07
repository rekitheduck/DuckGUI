#include "Window.h"

#include <iostream>

#include "wayland-client.h"

using namespace DuckGUI;

ReturnStatus Window::initDisplay() {
    m_display = wl_display_connect(nullptr);
    if (m_display == nullptr) {
        std::cout << "connection f ailed :(" << std::endl;
        return ReturnStatus::Bad;
    }

    // temp
    wl_display_disconnect(m_display);
    std::cout << "connection good :)" << std::endl;
    m_display = nullptr;

    return ReturnStatus::Good;
}
