set(PICTUREIT_TARGET ${CMAKE_BINARY_DIR}/3rdParty)

# Download and unpack PictureIt at configure time
configure_file(${CMAKE_CURRENT_LIST_DIR}/PictureIt-CMakeLists.txt.in
               ${PICTUREIT_TARGET}/PictureIt-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${PICTUREIT_TARGET}/PictureIt-download )

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${PICTUREIT_TARGET}/PictureIt-download )


# Add PictureIt directly to our build.
add_subdirectory(${PICTUREIT_TARGET}/PictureIt-src
                 ${PICTUREIT_TARGET}/PictureIt-build)
