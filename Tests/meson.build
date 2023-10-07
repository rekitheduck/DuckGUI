duckgui_tests_src = files ([
    'Catch2Main.cpp',
    'Platform/WindowTests.cpp'
])

duckgui_tests = executable('duckgui-test', sources: duckgui_tests_src, include_directories: duckgui_inc, dependencies: [libduckguiplatform_dep, catch2_dep])