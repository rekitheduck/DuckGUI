project('duckgui', 'cpp',
    version: '1.0',
    meson_version : '>= 0.63.0',
    license: 'LGPL-2.1-or-later'
)

# Various requirements
wayland_proto_req  = '>= 1.31'
wayland_req        = '>= 1.21.0'

wlclient_dep    = dependency('wayland-client', version:  wayland_req,
                              default_options: ['documentation=false'])
wlprotocols_dep = dependency('wayland-protocols', version: wayland_proto_req)

# TODO: Switch to "catch2-with-main" once that is available
catch2_dep      = dependency('catch2', required: true)

duckgui_inc = include_directories('.')

subdir('Platform')
# subdir('GUI')
if catch2_dep.found()
    subdir('Tests')
endif