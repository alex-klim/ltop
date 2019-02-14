cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb")

project(ltop)
find_package(Threads)
add_executable(ltop main.cpp app.cpp)

add_subdirectory(client)
add_subdirectory(ui)
target_include_directories(ltop PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ui/include)
target_include_directories(ltop PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/client/include)
target_link_libraries(ltop client ui)
target_link_libraries(ltop ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ltop stdc++fs)
