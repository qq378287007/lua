get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DIR_NAME})

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
)

add_subdirectory(lib)

#add_subdirectory(lua)
#add_subdirectory(luac)

add_subdirectory(callLua)


#add_subdirectory(callC)
#add_subdirectory(mylib)
