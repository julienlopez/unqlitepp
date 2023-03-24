set(EXE_TARGET_NAME unit_tests_unqlitepp)

find_package(Catch2 REQUIRED)

add_executable(${EXE_TARGET_NAME} "test_basic.cpp")

target_link_libraries(${EXE_TARGET_NAME} PRIVATE Catch2::Catch2WithMain unqlitepp project_options)

include(Catch)
catch_discover_tests(${EXE_TARGET_NAME})
