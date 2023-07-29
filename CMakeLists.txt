# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

# 打开 message 输出
set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

# 设置项目名称
set(skywalker_server_project_name "SkywalkerGameServer")

message(STATUS "Project Name: ${skywalker_server_project_name}")

# 项目名称
project(${skywalker_server_project_name})

# SkywalkerServerFramework
add_subdirectory(SkywalkerServerFramework)

# 服务器代码
add_subdirectory(Src)
