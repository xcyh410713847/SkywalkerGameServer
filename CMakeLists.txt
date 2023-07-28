# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

# 设置项目名称
set(skywalker_project_name "SkywalkerGameServer")

# 项目名称
project(${skywalker_project_name})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# SkywalkerServerFramework
add_subdirectory(skywalker-server-framework)

# 服务器代码
add_subdirectory(src)
