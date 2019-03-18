cmake_minimum_required(VERSION 3.12)
project(dlx-assembler)

set(CMAKE_CXX_STANDARD 14)

add_executable(assembler dlx_assembler.cpp tempMap.h)
