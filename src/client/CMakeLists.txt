cmake_minimum_required(VERSION 3.5)

project(client)
add_library(client STATIC client.cpp)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/include)
