cmake_minimum_required(VERSION 3.14)

project(sentinel-cpp VERSION 1.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

set(SENTINEL_CORE_ROOT_DIR "${CMAKE_SOURCE_DIR}/../sentinel-core")

set(SENTINEL_ROOT_DIR "${CMAKE_SOURCE_DIR}/..")

include( ${CMAKE_SOURCE_DIR}/../cmake/common.cmake )

# basic_qps_limit
add_executable(basic_qps_limit ${CMAKE_SOURCE_DIR}/../examples/sentinel-cpp/basic_qps_limit.cc)
target_link_libraries(basic_qps_limit sentinel)

# basic_concurrency_limit
add_executable(basic_concurrency_limit ${CMAKE_SOURCE_DIR}/../examples/sentinel-cpp/basic_concurrency_limit.cc)
target_link_libraries(basic_concurrency_limit sentinel)

# basic_param_limit
add_executable(basic_param_limit ${CMAKE_SOURCE_DIR}/../examples/sentinel-cpp/basic_param_limit.cc)
target_link_libraries(basic_param_limit sentinel)

# basic_system_limit
add_executable(basic_system_limit ${CMAKE_SOURCE_DIR}/../examples/sentinel-cpp/basic_system_limit.cc)
target_link_libraries(basic_system_limit sentinel)