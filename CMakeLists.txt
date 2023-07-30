# #########################################################################
# 文件: CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:43:06
# 功能:
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerGameServer_MessageDebug info)
    message(STATUS "[Skywalker Game Server] ${info}")
endfunction()

# 设置项目名称
set(skywalker_server_project_name "SkywalkerGameServer")

# 项目名称
project(${skywalker_server_project_name})

# SkywalkerServerFramework
add_subdirectory(SkywalkerServerFramework)

# 开始 SkywalkerGameServer 项目
SkywalkerGameServer_MessageDebug("Project Name: ${skywalker_server_project_name}")

# 服务器代码
add_subdirectory(Src)
