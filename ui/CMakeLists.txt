cmake_minimum_required(VERSION 3.8)
project(ui)
add_library(ui STATIC ui.cpp)
target_include_directories(ui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ui PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(ui PRIVATE ${CMAKE_SOURCE_DIR}/termbox/include)
find_library(libtermbox.a PATH ${CMAKE_SOURCE_DIR}/termbox/lib)
target_link_libraries(ui libtermbox.a)

