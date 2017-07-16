cmake_minimum_required(VERSION 3.5.0)

project(RDB_Diplomaterv_Monitor)
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")

set(COMPILER_FLAGS -O0 -g -std=c++14)


add_subdirectory(sources)
