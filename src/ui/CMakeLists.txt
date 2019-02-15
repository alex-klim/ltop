cmake_minimum_required(VERSION 3.8)
project(ui)
add_library(ui STATIC ui.cpp)
target_include_directories(ui PRIVATE ${CMAKE_SOURCE_DIR}/include)
find_library(libtermbox.a PATH ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(ui libtermbox.a)

