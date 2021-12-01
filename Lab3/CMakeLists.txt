cmake_minimum_required (VERSION 2.6)
project(Lab3)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

add_library(readfile ../common/readfile.cpp)
add_library(lodepng ../common/lodepng.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
set(common_libs ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} lodepng readfile)


add_executable(lab3-1 lab3-1.cpp)
add_executable(lab3-2 lab3-2.cpp)
add_executable(lab3-3 lab3-3.cpp)
add_executable(lab3-4 lab3-4.cpp)


target_link_libraries(lab3-1 ${common_libs})
target_link_libraries(lab3-2 ${common_libs})
target_link_libraries(lab3-3 ${common_libs})
target_link_libraries(lab3-4 ${common_libs})
