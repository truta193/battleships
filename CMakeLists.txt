cmake_minimum_required(VERSION 3.23)
project(battleships C)

set(CMAKE_C_STANDARD 23)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(battleships client.c ship.c ship.h common.h grid.c grid.h common.c)

target_link_libraries(battleships ${OPENGL_LIBRARIES})