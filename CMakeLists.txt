cmake_minimum_required(VERSION 3.14)

project(sentinel-cpp VERSION 1.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

set(SENTINEL_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(SENTINEL_CORE_ROOT_DIR "${CMAKE_SOURCE_DIR}/sentinel-core")

include( cmake/sentinel_core.cmake )