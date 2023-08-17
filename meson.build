project('Tetris2.0', 'cpp')

incdir = include_directories('src/lib/imgui/backends', 'src/lib/implot', 'src/lib/imgui', 'src/lib')

glfw_lib = subproject('glfw').get_variable('glfw_dep')
gl = dependency('gl')
catch_lib = subproject('catch2').get_variable('catch2_with_main_dep')

gui_files = files(
    'src/lib/imgui/imgui.cpp',
    'src/lib/imgui/imgui_draw.cpp',
    'src/lib/imgui/imgui_tables.cpp',
    'src/lib/imgui/imgui_demo.cpp',
    'src/lib/imgui/imgui_widgets.cpp',
    'src/lib/imgui/backends/imgui_impl_glfw.cpp',
    'src/lib/imgui/backends/imgui_impl_opengl3.cpp',
    'src/lib/implot/implot.cpp',
    'src/lib/implot/implot_items.cpp'
)

subdir('src/app/tetris')

subdir('src/lib/thread_pool')