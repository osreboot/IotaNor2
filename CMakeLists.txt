cmake_minimum_required(VERSION 3.25)
project(CppWarmup)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(src)

find_package(OpenGL REQUIRED)
add_subdirectory(glfw)

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw)