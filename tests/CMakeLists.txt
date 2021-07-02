cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(vendor)
add_executable(test test.cpp)

#Enables including files in specified directories.
target_include_directories(test
       PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(test gtest gtest_main)