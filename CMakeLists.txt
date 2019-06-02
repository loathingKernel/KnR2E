cmake_minimum_required(VERSION 3.0)

project(knrse LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")

add_executable(ex1-23_a ex1-23/ex1-23_a.c)
add_executable(ex1-23_b ex1-23/ex1-23_b.c)

install(TARGETS ex1-23_a RUNTIME DESTINATION bin)
install(TARGETS ex1-23_b RUNTIME DESTINATION bin)
