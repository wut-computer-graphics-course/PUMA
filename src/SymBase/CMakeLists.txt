project(SymBase)

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
        glfw
        glm
        imgui
        implot
        stb_image
)
