cmake_minimum_required(VERSION 3.29)
project(estrutura_dados_at1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(estrutura_dados_at1
    main.c
    processo.c
    processo.h)
