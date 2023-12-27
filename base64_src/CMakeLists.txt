cmake_minimum_required(VERSION 3.16)

project(
    Base64Lib
    LANGUAGES C
)

add_library(base64 STATIC src/base64.c)
target_include_directories(base64 PUBLIC inc)