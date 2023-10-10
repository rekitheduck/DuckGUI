duckgui_platform_src = files ([
    'Window.cpp',
    'xdg-shell-protocol.c'
])

duckgui_platform_headers = files ([
    'Window.h',
    'xdg-shell-client-protocol.h'
])

libduckguiplatform = static_library('duckguiplatform', sources: [duckgui_platform_src, duckgui_platform_headers], dependencies: [wlclient_dep, wlprotocols_dep])

libduckguiplatform_dep = declare_dependency(
    link_with: libduckguiplatform
)