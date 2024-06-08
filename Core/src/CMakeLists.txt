macro(RUNTIME_ADD_SOURCE)
	set(SOURCE_FILES "")
    foreach(SOURCE_FILE ${ARGN})
        if(SOURCE_FILE MATCHES "^/.*")
            list(APPEND SOURCE_FILES ${SOURCE_FILE})
        else()
            list(APPEND SOURCE_FILES
                 "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")
        endif()
    endforeach()
    set(CoreSourceFiles ${CoreSourceFiles} ${SOURCE_FILES} PARENT_SCOPE)
endmacro(RUNTIME_ADD_SOURCE)

add_subdirectory("Math")

#cmake非官方旧式命令，辅助模块的函数，不建议在新项目中使用
#aux_source_directory(. CORE_SOURCE_DIR)
#file(GLOB CORE_SOURCES "src/*.cpp")
add_library(Core STATIC ${CoreSourceFiles})

#向目标指定头文件
target_include_directories(Core PUBLIC .)