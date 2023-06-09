get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DIR_NAME})

add_subdirectory(libLua)

file(GLOB ALL_FILE "*.h" "*.cpp")
add_executable(${DIR_NAME} ${ALL_FILE})

find_package(Qt5 REQUIRED COMPONENTS Widgets)
target_link_libraries(${DIR_NAME} libLua Qt5::Widgets)
