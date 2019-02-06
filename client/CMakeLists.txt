cmake_minimum_required(VERSION 3.12)

project(client)
#add_executable(test main.cpp client.cpp)
add_library(client STATIC client.cpp)
#target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../)
