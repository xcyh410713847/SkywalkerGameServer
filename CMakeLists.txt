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

# SkywalkerServerFramework
add_subdirectory(SkywalkerServerFramework)

# SkywalkerServerEditor
add_subdirectory(SkywalkerServerEditor)

# SkywalkerGameServer
add_subdirectory(SkywalkerGameServer)

# SkywalkerGameClient
add_subdirectory(SkywalkerGameClient)
