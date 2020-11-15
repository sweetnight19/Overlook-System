cmake_minimum_required(VERSION 3.11)

# set the project name
project(Danny)

# add the executable
add_executable(Danny danny.c configuracion.h configuracion.c ../Jack/jack.c ../Jack/configuracion.c ../Jack/configuracion.h ../Jack/servidor.h client.h)