# #########################################################################
# 文件: Src\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:42:34
# 功能:
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

function(SkywalkerGameServer_MessageDebug info)
    message(STATUS "[Skywalker Game Server] ${info}")
endfunction()

# 设置项目名称
set(skywalker_server_project_name "SkywalkerGameServer")

# 项目名称
project(${skywalker_server_project_name})

SkywalkerGameServer_MessageDebug("Start Build SkywalkerGameServer")

# 头文件目录
include_directories(
    ../
    ./Include
)

# 服务器模块文件夹
add_subdirectory(Module)

# 执行文件文件夹
add_subdirectory(Executable)

SkywalkerGameServer_MessageDebug("End Build SkywalkerGameServer")
