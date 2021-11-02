cmake_minimum_required (VERSION 2.6)
project(Lab1)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

add_library(readfile ../common/readfile.cpp)
add_library(lodepng ../common/lodepng.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
set(common_libs ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} lodepng readfile)


add_executable(lab1-1 lab1-1.cpp)
add_executable(lab1-2 lab1-2.cpp)
add_executable(lab1-3 lab1-3.cpp)
add_executable(lab1-4 lab1-4.cpp)
add_executable(lab1-5 lab1-5.cpp)
add_executable(lab1-6 lab1-6.cpp)
add_executable(lab1-7 lab1-7.cpp)

target_link_libraries(lab1-1 ${common_libs})
target_link_libraries(lab1-2 ${common_libs})
target_link_libraries(lab1-3 ${common_libs})
target_link_libraries(lab1-4 ${common_libs})
target_link_libraries(lab1-5 ${common_libs})
target_link_libraries(lab1-6 ${common_libs})
target_link_libraries(lab1-7 ${common_libs})
