cmake_minimum_required(VERSION 3.23)
project(lab_7_virtual_mem_linux C)

set(CMAKE_C_STANDARD 99)

add_executable(lab_7_virtual_mem_linux shmem.h main.c)
