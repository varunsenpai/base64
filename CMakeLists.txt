cmake_minimum_required(VERSION 3.16)

project(
    app
    LANGUAGES C
)

add_executable(base64_app app.c)
target_include_directories(base64_app PUBLIC base64_src/inc)

set(base64_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/base64_src/build)

target_link_libraries(base64_app PRIVATE ${base64_lib_dir}/libbase64.a)