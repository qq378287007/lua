get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DIR_NAME})

file(GLOB ALL_FILE "*.c")
add_executable(${DIR_NAME} ${ALL_FILE})

target_link_libraries(${DIR_NAME} lib)

file(COPY test.lua DESTINATION ${CMAKE_BINARY_DIR}/${DIR_NAME})
