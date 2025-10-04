cmake_minimum_required(VERSION 3.29)
project(P3 C)

set(CMAKE_C_STANDARD 99)

add_executable(P3
        p3.c
        cmds.c
        openfiles.c
        historic.c
        aux.c
        memory.c
        jobs.c
        dirs.c)
