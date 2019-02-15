cmake_minimum_required(VERSION 3.12)

project(client)
add_library(client STATIC client.cpp)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})
