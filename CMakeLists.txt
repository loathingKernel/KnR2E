cmake_minimum_required(VERSION 3.0)

project(knrse LANGUAGES C)

add_executable(knrse main.c)

install(TARGETS knrse RUNTIME DESTINATION bin)
