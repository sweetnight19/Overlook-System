cmake_minimum_required(VERSION 3.17)
project(Jack C)

set(CMAKE_C_STANDARD 11)

add_executable(Jack jack.c configuracion.c configuracion.h servidor.c servidor.h)