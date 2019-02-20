cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

project(ltop)
find_package(Threads)
file(GLOB APP_SRC src/*.cpp)
link_directories(AFTER ${CMAKE_SOURCE_DIR}/lib)
add_executable(ltop ${APP_SRC})
target_include_directories(ltop PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src/client)
add_subdirectory(src/ui)
target_link_libraries(ltop client ui)
target_link_libraries(ltop ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ltop stdc++fs)

