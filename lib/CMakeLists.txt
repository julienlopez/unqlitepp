set(LIB_TARGET_NAME unqlitepp)

set(SOURCE_FILES "unqlitepp/unqlitepp.cpp")

add_library(${LIB_TARGET_NAME} ${SOURCE_FILES})

target_include_directories(${LIB_TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(
  ${LIB_TARGET_NAME}
  PUBLIC tl::expected unqlite::unqlite
  PRIVATE project_options)
