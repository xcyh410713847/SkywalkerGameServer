# 头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 服务器模块文件夹
add_subdirectory(Module)

# 执行文件文件夹
add_subdirectory(Executable)
