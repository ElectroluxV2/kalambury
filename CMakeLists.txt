cmake_minimum_required(VERSION 3.17)
project(Projekt1)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(Projekt1 POP_2020-12-07_projekt_1_Budzisz_Mateusz_EiT_2_184325.cpp)

configure_file(data.yml data.yml COPYONLY)