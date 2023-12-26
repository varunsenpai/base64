cmake_minimum_required(VERSION 3.16)
set(CMAKE_BUILD_TYPE Debug)

project(
    app
    LANGUAGES C
)

add_library(aes STATIC src/aes.c)
target_include_directories(aes PUBLIC inc)