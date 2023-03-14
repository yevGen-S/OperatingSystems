cmake_minimum_required(VERSION 3.23)
project(LAB5_DEBUG C)

set(CMAKE_C_STANDARD 99)

add_executable(LAB5_DEBUG producer_consimer.c ring_buffer.c ring_buffer.h)
