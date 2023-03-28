set(EXE_TARGET_NAME unqlitepp_example)

add_executable(${EXE_TARGET_NAME} "main.cpp")

target_link_libraries(${EXE_TARGET_NAME} PRIVATE unqlitepp project_options)
