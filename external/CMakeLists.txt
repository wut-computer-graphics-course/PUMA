
# ------------------------------ INCLUDE GLAD ------------------------------
add_subdirectory(glad)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE GLFW ------------------------------
add_subdirectory(glfw)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE GLM ------------------------------
add_subdirectory(glm)
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE IMGUI ------------------------------
set(IMGUI_PATH "imgui")

file(GLOB IMGUI_SOURCES
        ${IMGUI_PATH}/*.cpp
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC ${IMGUI_PATH})
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE IMPLOT ------------------------------
set(IMPLOT_PATH "implot")

file(GLOB IMPLOT_SOURCES ${IMPLOT_PATH}/*.cpp)

add_library(implot STATIC ${IMPLOT_SOURCES})

target_include_directories(implot PUBLIC ${IMGUI_PATH})
# -------------------------------------------------------------------------

# ------------------------------ INCLUDE STB ------------------------------
add_subdirectory(stb_image)
# -------------------------------------------------------------------------
