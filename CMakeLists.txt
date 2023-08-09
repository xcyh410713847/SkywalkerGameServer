# #########################################################################
# 文件: CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:43:06
# 功能:
# #########################################################################
# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

# 项目名称
project(SkywalkerServer)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# 设置导出路径
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(Skywalker_Server_output_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Debug CACHE STRING "Skywalker_Server_output_PATH")
else()
    set(Skywalker_Server_output_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Release CACHE STRING "Skywalker_Server_output_PATH")
endif()

# SkywalkerServerFramework
add_subdirectory(SkywalkerServerFramework)

# SkywalkerServerEditor
add_subdirectory(SkywalkerServerEditor)

# SkywalkerGameServer
add_subdirectory(SkywalkerGameServer)

# SkywalkerGameClient
add_subdirectory(SkywalkerGameClient)
