cmake_minimum_required(VERSION 3.23)
project(CppWarmup)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)
add_subdirectory(src)

add_subdirectory(glfw)

add_definitions(-DGLEW_STATIC)
add_subdirectory(glew)

target_link_libraries(${PROJECT_NAME} glfw libglew_static)