#include "catch2/catch.hpp"

#include "Platform/Window.h"

TEST_CASE("Creating and destroying a Wayland display", "[Platform]") {
    DuckGUI::Window window;

    const auto status = window.initDisplay();

    REQUIRE(status == DuckGUI::ReturnStatus::Good);
}