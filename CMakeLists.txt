cmake_minimum_required(VERSION 3.16)
set(CMAKE_BUILD_TYPE Debug)

project(
    app
    LANGUAGES C
)

add_executable(base64_app app.c)
target_include_directories(base64_app PUBLIC base64_src/inc aes/inc)

set(base64_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/base64_src/build)
set(aes_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/aes/build)

target_link_libraries(base64_app PRIVATE ${base64_lib_dir}/libbase64.a ${aes_lib_dir}/libaes.a)