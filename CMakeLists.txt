cmake_minimum_required(VERSION 3.16)
set(CMAKE_BUILD_TYPE Debug)

project(
    app
    LANGUAGES C
)

add_executable(base64_app app.c)
target_compile_options(base64_app PRIVATE -g)
add_subdirectory(aes)
add_subdirectory(base64_src)
add_subdirectory(token_service)
target_include_directories(base64_app PUBLIC base64_src/inc aes/inc token_service/inc)

set(base64_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/base64_src/build)
set(aes_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/aes/build)
set(token_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/token_service/build)

target_link_libraries(base64_app PRIVATE ${base64_lib_dir}/libbase64.a ${aes_lib_dir}/libaes.a ${token_lib_dir}/libtoken.a)