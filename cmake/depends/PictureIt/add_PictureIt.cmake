set(PICTUREIT_SRC_DIR ${CMAKE_BINARY_DIR}/depends/PictureIt)
set(PICTUREIT_BUILD_DIR ${CMAKE_BINARY_DIR}/depends/0build/PictureIt)

# Download and unpack PictureIt at configure time
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in
               ${PICTUREIT_BUILD_DIR}/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                WORKING_DIRECTORY ${PICTUREIT_BUILD_DIR})

execute_process(COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${PICTUREIT_BUILD_DIR})


add_subdirectory(${PICTUREIT_SRC_DIR}
                 ${PICTUREIT_BUILD_DIR})


mark_as_advanced(FORCE ${PICTUREIT_SRC_DIR} ${PICTUREIT_BUILD_DIR})
