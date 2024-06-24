cmake_minimum_required(VERSION 3.0)

add_subdirectory("01 Graphics Algorithms")
add_subdirectory("02 Path Finding")


# you maybe need assets file and want to copy it to cmake binary dir
#if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Texture)
	#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Texture DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#endif()