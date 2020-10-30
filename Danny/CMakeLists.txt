cmake_minimum_required(VERSION 3.11)

# set the project name
project(Danny)

# add the executable
add_executable(Danny danny.c Configuracion/configuracion.h Configuracion/configuracion.c)