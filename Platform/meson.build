duckgui_platform_src = files ([
    'Window.cpp'
])

duckgui_platform_headers = files ([
    'Window.h'
])

libduckguiplatform = static_library('duckguiplatform', sources: [duckgui_platform_src, duckgui_platform_headers], dependencies: [wlclient_dep])

libduckguiplatform_dep = declare_dependency(
    link_with: libduckguiplatform
)