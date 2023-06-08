get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DIR_NAME})

file(GLOB ALL_FILE "*.h" "*.c")
add_library(${DIR_NAME} ${ALL_FILE})
