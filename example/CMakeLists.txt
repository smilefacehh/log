cmake_minimum_required(VERSION 3.0.0)
project(example)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Werror -Wno-error=unused-variable -O3 -fPIC)

include_directories(../install/include/)

add_executable(example main.cpp)
#target_link_libraries(example /home/lutao/workspace/me/log/install/lib/libNlog.a)