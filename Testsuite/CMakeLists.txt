cmake_minimum_required(VERSION 3.8)
project(Desktop_Testsuite)

find_package(SDL2 REQUIRED)

set(OpenGL_GL_PREFERENCE "GLVND")
# OpenGL
find_package(OpenGL REQUIRED)

add_subdirectory(libs/Boost)

file(GLOB header_list_imgui CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/libs/imgui/")
file(GLOB header_list_imgui_backend CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/libs/imgui/backends/")
set(imgui_header_list "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_internal.h"
                      "${PROJECT_SOURCE_DIR}/libs/imgui/imconfig.h"
                      "${PROJECT_SOURCE_DIR}/libs/imgui/imgui.h"
                      "${PROJECT_SOURCE_DIR}/libs/imgui/imstb_rectpack.h"
                      "${PROJECT_SOURCE_DIR}/libs/imgui/imstb_textedit.h"
                      "${PROJECT_SOURCE_DIR}/libs/imgui/imstb_truetype.h")
set(imgui_source_list_core  "${PROJECT_SOURCE_DIR}/libs/imgui/imgui.cpp" 
                            "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp" 
                            "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_demo.cpp"
                            "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_tables.cpp"
                            "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp")
set(imgui_source_list "${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_sdl2.cpp"
                "${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.cpp")

add_library(imgui ${imgui_source_list} ${header_list_imgui_backend}) 
add_library(imgui_core ${imgui_header_list} ${imgui_source_list_core})
target_link_libraries(imgui PUBLIC SDL2::SDL2)
target_link_libraries(imgui PUBLIC imgui_core)
target_include_directories(imgui PUBLIC "${PROJECT_SOURCE_DIR}/libs/imgui"
                                        "${PROJECT_SOURCE_DIR}/libs/imgui/backends")

set(implot_header_list "${PROJECT_SOURCE_DIR}/libs/implot/implot.h"
                       "${PROJECT_SOURCE_DIR}/libs/implot/implot_internal.h")
set(implot_source_list "${PROJECT_SOURCE_DIR}/libs/implot/implot.cpp"
                       "${PROJECT_SOURCE_DIR}/libs/implot/implot_items.cpp"
                       "${PROJECT_SOURCE_DIR}/libs/implot/implot_demo.cpp")
add_library(implot ${implot_header_list} ${implot_source_list})
target_include_directories(implot PUBLIC "${PROJECT_SOURCE_DIR}/libs/implot")

target_link_libraries(implot PRIVATE imgui)

add_executable(app main.cpp gui_setup.h src/communication.cpp src/communication.hpp)
target_include_directories(app PUBLIC src)

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(app PRIVATE SDL2::SDL2)
target_link_libraries(app PRIVATE GL)

target_link_libraries(app PUBLIC imgui)

target_link_libraries(app PUBLIC implot)
target_link_libraries(app PUBLIC boost_filesystem)
target_link_libraries(app PUBLIC boost_chrono)
target_link_libraries(app PRIVATE boost_asio)