cmake_minimum_required(VERSION 3.0)

add_subdirectory("01 Object-Oriented Programming")
add_subdirectory("02 Memory Management")
add_subdirectory("03 Smart Pointer")
add_subdirectory("04 Design Pattern")
add_subdirectory("05 Type Conversion")
#add_subdirectory("08 Macros")

# you maybe need assets file and want to copy it to cmake binary dir
#if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Texture)
	#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Texture DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#endif()