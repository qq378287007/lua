get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DIR_NAME})

include_directories(${CMAKE_SOURCE_DIR}/libLua)

add_subdirectory(libLua)

file(GLOB ALL_FILE "*.cpp")
add_executable(${DIR_NAME} ${ALL_FILE})
target_link_libraries(${DIR_NAME} libLua)
