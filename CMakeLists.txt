cmake_minimum_required(VERSION 3.25.0)
project(planning
    VERSION 0.0.1
    DESCRIPTION "a demo of cmake planning"
    HOMEPAGE_URL "https://github.com/1009962019/cmake_template"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 11)
enable_testing()

# 变量和目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# 添加子目录
add_subdirectory(src)