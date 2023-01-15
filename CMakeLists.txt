cmake_minimum_required(VERSION 3.24)
project(SistemaTienda C)

set(CMAKE_C_STANDARD 99)

enable_testing()

add_subdirectory(src bin)
add_subdirectory(Test)