﻿# #########################################################################
# 文件: Src\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:42:34
# 功能: Skywalker Game Server 项目CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerGameServer_MessageDebug info)
    message(STATUS "[Skywalker Game Server] ${info}")
endfunction()

# 设置项目名称
set(skywalker_game_server_name "SkywalkerGameServer")

# 项目名称
project(${skywalker_game_server_name})

SkywalkerGameServer_MessageDebug("Start Build ${skywalker_game_server_name}")

# 导出宏
macro(SkywalkerGameServer_Export TargetName)
    SkywalkerServer_Export(${TargetName} ${skywalker_game_server_name})
endmacro()

# 包含路径
set(skywalker_server_path "../")
set(skywalker_tools_path "../SkywalkerTools")
set(skywalker_server_framework_path "../SkywalkerServerFramework")

# 头文件目录
include_directories(
    ${skywalker_server_path}
    ${skywalker_tools_path}
    ${skywalker_server_framework_path}
)

# 创建执行文件
add_executable(${skywalker_game_server_name}
    SkywalkerGameServer.cpp
)

target_link_libraries(${skywalker_game_server_name} PRIVATE
    ${skywalker_server_framework_name}
)

SkywalkerGameServer_Export(${skywalker_game_server_name})

SkywalkerServer_CopyToTarget(${skywalker_game_server_name} ${skywalker_server_server_dir})

SkywalkerGameServer_MessageDebug("End Build ${skywalker_game_server_name}")
