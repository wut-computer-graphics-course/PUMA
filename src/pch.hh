#ifndef OPENGL_BASE_PCH_HH
#define OPENGL_BASE_PCH_HH

#include <chrono>
#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <string>
#include <thread>

// glad
#include <glad/glad.h>

// glfw
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>

// imgui
#include <external/imgui/backends/imgui_impl_glfw.h>
#include <external/imgui/backends/imgui_impl_opengl3.h>
#include <external/imgui/imgui.h>

// implot
#include <external/implot/implot.h>

#endif // OPENGL_BASE_PCH_HH
