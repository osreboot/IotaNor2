cmake_minimum_required(VERSION 3.23)
project(CppWarmup)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(lib)
include_directories(src)

add_subdirectory(src)

add_subdirectory(lib/glfw)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew)

target_link_libraries(${PROJECT_NAME} glfw libglew_static)