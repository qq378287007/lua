get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DIR_NAME})

file(GLOB ALL_FILE "*.c")
add_library(${DIR_NAME} SHARED ${ALL_FILE})
target_link_libraries(${DIR_NAME} lib)

file(COPY a.lua DESTINATION ${CMAKE_BINARY_DIR}/${DIR_NAME})
