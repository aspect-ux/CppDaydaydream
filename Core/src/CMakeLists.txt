cmake_minimum_required(VERSION 3.0)

aux_source_directory(. CORE_SOURCE_DIR)
add_library(Core STATIC ${CORE_SOURCE_DIR})

#向目标指定头文件
target_include_directories(Core PUBLIC .)