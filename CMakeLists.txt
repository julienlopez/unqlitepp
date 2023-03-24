cmake_minimum_required(VERSION 3.20)

project(unqlitepp LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(unqlite REQUIRED)
find_package(tl-expected REQUIRED)

add_library(project_options INTERFACE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

  add_definitions("-DNOMINMAX")
endif()

# subdirs
add_subdirectory(lib)

option(BUILD_TESTING "Enable unit tests" ON)
if(BUILD_TESTING)
  include(CTest)
  add_subdirectory(unit_tests)
endif()
