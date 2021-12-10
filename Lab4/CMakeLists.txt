cmake_minimum_required (VERSION 2.6)
project(Lab4)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

add_library(readfile ../common/readfile.cpp)
add_library(lodepng ../common/lodepng.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
set(common_libs ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} lodepng readfile)

add_executable(lab4-1 lab4-1.cpp)
add_executable(lab4-2 lab4-2.cpp)
add_executable(lab4-3 lab4-3.cpp)
add_executable(lab4-4 lab4-4.cpp)

target_link_libraries(lab4-1 ${common_libs})
target_link_libraries(lab4-2 ${common_libs})
target_link_libraries(lab4-3 ${common_libs})
target_link_libraries(lab4-4 ${common_libs})
