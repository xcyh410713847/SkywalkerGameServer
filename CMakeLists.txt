# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

# 打开 message 输出
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

SkywalkerGameServer_MessageDebug("Project Name: ${skywalker_server_project_name}")

# 服务器代码
add_subdirectory(Src)
