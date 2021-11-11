cmake_minimum_required (VERSION 2.6)
project(Lab2)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
add_library(readfile ../common/readfile.cpp)
add_library(lodepng ../common/lodepng.cpp)
set(common_libs ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} lodepng readfile)

add_executable(lab2-1 lab2-1.cpp)
add_executable(lab2-2 lab2-2.cpp)
add_executable(lab2-3 lab2-3.cpp)
add_executable(lab2-4 lab2-4.cpp)


target_link_libraries(lab2-1 ${common_libs})
target_link_libraries(lab2-2 ${common_libs})
target_link_libraries(lab2-3 ${common_libs})
target_link_libraries(lab2-4 ${common_libs})

