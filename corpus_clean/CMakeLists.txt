cmake_minimum_required(VERSION 3.1)
project(PREPROCESSING)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")

include_directories("./include")
include_directories("./third_party/cppjieba")

add_subdirectory(src bin)
add_executable(multi_prep "src/main.cpp")

target_link_libraries(multi_prep iniconfig)
target_link_libraries(multi_prep threadpool)
target_link_libraries(multi_prep processcorpus)