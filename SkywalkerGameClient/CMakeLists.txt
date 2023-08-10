# #########################################################################
# 文件: SkywalkerGameClient\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/07 11:29:57
# 功能: Skywalker Game Client 项目CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerGameClient_MessageDebug info)
    message(STATUS "[Skywalker Game Client] ${info}")
endfunction()

# 设置项目名称
set(skywalker_game_client_name "SkywalkerGameClient")

# 项目名称
project(${skywalker_game_client_name})

SkywalkerGameClient_MessageDebug("Start Build ${skywalker_game_client_name}")

# 导出宏
macro(SkywalkerGameClient_Export TargetName)
    SkywalkerServer_Export(${TargetName} ${skywalker_game_client_name})
endmacro()

# 支持C++17
set(CMAKE_CXX_STANDARD 17)

# SkywalkerTools 路径
set(skywalker_tools_path "../SkywalkerTools")

# 头文件目录
include_directories(
    ${skywalker_tools_path}
    ../
)

# 生成可执行文件
add_executable(${skywalker_game_client_name}
    SkywalkerGameClient.cpp
)

SkywalkerGameClient_Export(${skywalker_game_client_name})

# 关闭警告
target_compile_options(${skywalker_game_client_name} PRIVATE "/wd4819")

SkywalkerGameClient_MessageDebug("End Build ${skywalker_game_client_name}")
