cmake_minimum_required(VERSION 3.27)
project(SimulationApp)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/SymSandbox/*.cc)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cc)

add_subdirectory(src/SymBase)

target_link_libraries(${PROJECT_NAME} SymBase)
