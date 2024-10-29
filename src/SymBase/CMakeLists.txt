project(SymBase)

# ------------------------------ INCLUDE OpenGl ------------------------------
find_package(OpenGL REQUIRED)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE GLAD ------------------------------
set(GLAD_SRC_DIR "${CMAKE_SOURCE_DIR}/external/glad")

add_library(glad STATIC ${GLAD_SRC_DIR}/src/glad.c)

target_include_directories(glad PUBLIC ${GLAD_SRC_DIR}/include)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE GLFW ------------------------------
find_package(glfw3 3.3 REQUIRED)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE GLM ------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glm)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE IMGUI ------------------------------
set(IMGUI_PATH "${CMAKE_SOURCE_DIR}/external/imgui")

file(GLOB IMGUI_SOURCES
        ${IMGUI_PATH}/*.cpp
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)

add_library(IMGUI STATIC ${IMGUI_SOURCES})

set(IMGUI_LIBRARIES IMGUI)

target_include_directories(IMGUI PUBLIC ${IMGUI_PATH})
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE IMPLOT ------------------------------
set(IMPLOT_PATH "${CMAKE_SOURCE_DIR}/external/implot")

file(GLOB IMPLOT_SOURCES ${IMPLOT_PATH}/*.cpp)

add_library(IMPLOT STATIC ${IMPLOT_SOURCES})

set(IMPLOT_LIBRARIES IMPLOT)

target_include_directories(IMPLOT PUBLIC ${IMGUI_PATH})
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE STB ------------------------------
set(STB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/stb_image")
include_directories(${STB_INCLUDE_DIR})
link_directories(${STB_INCLUDE_DIR})
# -------------------------------------------------------------------------

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets/")
add_definitions(-DASSETS_DIR="${ASSETS_DIR}")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cc)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Core)

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hh)

target_link_libraries(
        ${PROJECT_NAME}
        glad
        OpenGL::GL
        glfw
        ${GLM_LIBRARIES}
        ${IMGUI_LIBRARIES}
        ${IMPLOT_LIBRARIES}
)
