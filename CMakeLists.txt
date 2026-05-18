cmake_minimum_required(VERSION 3.10)
project(MindMatrixCAD C)

set(CMAKE_C_STANDARD 99)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Hozzáadjuk a generált glad include mappát is a kereséshez
include_directories(
    include
    third_party/include
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
)

# Matematikai modul
add_library(mmc_math STATIC 
    src/math/vector_nd.c 
    src/math/matrix_nd.c 
    src/math/projection.c
)

# Geometria modul
add_library(mmc_geometry STATIC 
    src/geometry/geometry_base.c 
    src/geometry/mesh_nd.c 
    src/geometry/parametric.c
    src/geometry/sdf.c
)

# Renderelő modul - A Glad 2 által generált gl.c használatával
add_library(mmc_render STATIC 
    src/render/renderer.c 
    src/render/shader.c 
    src/render/camera.c
    third_party/src/gl.c     # <-- EZT ÍRTUK ÁT!
)

# UI és Scripting modulok
add_library(mmc_core STATIC 
    src/core/app.c 
    src/core/engine.c
    src/ui/gui_manager.c
    src/ui/panel_properties.c
    src/ui/panel_console.c
    src/scripting/compiler_bridge.c
    src/scripting/plugin_manager.c
    src/scripting/formula_parser.c
)

add_executable(mind_matrix_cad apps/main_app/main.c)

target_link_libraries(mind_matrix_cad 
    mmc_core 
    mmc_render 
    mmc_geometry 
    mmc_math 
    OpenGL::OpenGL    # <-- `${OPENGL_LIBRARIES}` HELYETT EZT ÍRD BE!
    ${GLFW_LIBRARIES}
    m 
    dl 
)
