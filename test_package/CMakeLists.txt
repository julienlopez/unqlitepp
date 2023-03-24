cmake_minimum_required(VERSION 3.20)
project(unqliteppPackageTest CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(unqlitepp REQUIRED)

set(EXE_TARGET_NAME unqlitepp_test)

add_executable(${EXE_TARGET_NAME} "unqlitepp_test.cpp")

target_link_libraries(${EXE_TARGET_NAME} PRIVATE unqlitepp::unqlitepp)
