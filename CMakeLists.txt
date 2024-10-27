cmake_minimum_required(VERSION 3.27)
project(sym_base)

set(CMAKE_CXX_STANDARD 20)

# ------------------------------ INCLUDE OpenGl ------------------------------
find_package(OpenGL REQUIRED)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE GLFW ------------------------------
find_package(glfw3 3.3 REQUIRED)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE GLM ------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glm)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE IMGUI ------------------------------
set(IMGUI_PATH "${PROJECT_SOURCE_DIR}/external/imgui")

file(GLOB IMGUI_SOURCES
        ${IMGUI_PATH}/*.cpp
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)

add_library(IMGUI STATIC ${IMGUI_SOURCES})

set(IMGUI_LIBRARIES IMGUI)

target_include_directories(IMGUI PUBLIC ${IMGUI_PATH})
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE IMPLOT ------------------------------
set(IMPLOT_PATH "${PROJECT_SOURCE_DIR}/external/implot")

file(GLOB IMPLOT_SOURCES ${IMPLOT_PATH}/*.cpp)

add_library(IMPLOT STATIC ${IMPLOT_SOURCES})

set(IMPLOT_LIBRARIES IMPLOT)

target_include_directories(IMPLOT PUBLIC ${IMGUI_PATH})
# -------------------------------------------------------------------------

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/Core)

add_executable(${PROJECT_NAME} ${SOURCES})

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hh)

target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        glfw
        ${GLM_LIBRARIES}
        ${IMGUI_LIBRARIES}
        ${IMPLOT_LIBRARIES}
)
