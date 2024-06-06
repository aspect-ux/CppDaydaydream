cmake_minimum_required(VERSION 3.0)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

# 向目标(库或可执行文件 添加编译选项)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 将当前目录源文件添加到DIR_SRCS
aux_source_directory(. DIR_SRCS)
# GLOB表示按照指定模式进行全局匹配，这里将当前目录头文件设置成HEADER_FILES
# 另，官方不推荐GLOB,而是显示将所有文件罗列出来...，因为只有运行cmake时才会自动添加，日常使用增添文件都无法被检测到
# 这里写好一个子项目后，基本不会更改，所以使用file(GLOB)
file(GLOB HEADER_FILES ./*.h)

# 添加可执行文件并根据需求链接库
add_executable(01_Object_Pool WIN32 ${DIR_SRCS} ${HEADER_FILES})
target_link_libraries(01_Object_Pool Core)

# 设置链接器选项为/SUBSYSTEM:CONSOLE（对于MSVC）  
if(MSVC)  
    target_link_options(01_Object_Pool PRIVATE "/SUBSYSTEM:CONSOLE")  
endif()  

#set_target_properties用于设置目标属性，也就是可执行文件或库的特性，例如链接器，编译器或安装路径等
set_target_properties(01_Object_Pool PROPERTIES OUTPUT_NAME "01 Object Pool")

set_target_properties(01_Object_Pool PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(01_Object_Pool PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(01_Object_Pool PROPERTIES FOLDER "STLDataStructures")


