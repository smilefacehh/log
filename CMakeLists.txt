cmake_minimum_required(VERSION 3.0.0)
project(main)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")

include_directories(spdlog/include)
link_directories(spdlog/build)


add_executable(main main.cpp)
target_link_libraries(main libspdlog.a)