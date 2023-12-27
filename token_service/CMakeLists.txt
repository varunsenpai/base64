cmake_minimum_required(VERSION 3.16)

project(
    TokenLib
    LANGUAGES C
)

add_library(token STATIC src/srv_token.c)
target_compile_options(token PUBLIC -g)
target_include_directories(token PUBLIC inc)