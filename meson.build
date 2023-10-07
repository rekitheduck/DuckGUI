project('duckgui', 'cpp',
        version: '1.0',
        meson_version : '>= 0.63.0',
        license: 'LGPL-2.1-or-later')

# Various requirements
wayland_proto_req  = '>= 1.31'
wayland_req        = '>= 1.21.0'

wlclientdep    = dependency('wayland-client', version:  wayland_req,
                              default_options: ['documentation=false'])
wlprotocolsdep = dependency('wayland-protocols', version: wayland_proto_req)

subdir('Platform')
# subdir('GUI')