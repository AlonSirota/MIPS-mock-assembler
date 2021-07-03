cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(vendor)
add_executable(tests test.cpp)
add_test(NAME check COMMAND tests)
#Enables including files in specified directories.
target_include_directories(tests
       PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/../src)

target_link_libraries(tests gtest gtest_main)