# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(skywalker_project_name "SkywalkerGameServer")

# 项目名称
project(${skywalker_project_name})

# 服务器代码
add_subdirectory(src)
