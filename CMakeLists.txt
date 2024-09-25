cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Debug)
project(game)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS}/home/jinguumirai/stb_master)
file(GLOB sources *.cpp *.hpp)
add_executable(game glad.c ${sources})
target_link_libraries(${PROJECT_NAME} ${OpenGL_LIBRARIES} ${OpenCV_LIBRARIES} glfw dl assimp)
