# 头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 服务器模块文件夹
add_subdirectory(module)

# 执行文件文件夹
add_subdirectory(executable)
