# #########################################################################
# 文件: SkywalkerScript\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/28 22:49:27
# 功能: Skywalker Script 项目CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerScript_MessageDebug info)
    message(STATUS "[Skywalker Script] ${info}")
endfunction()

# 设置项目名称
set(skywalker_script_name "SkywalkerScript")

# 项目名称
project(${skywalker_script_name})

SkywalkerScript_MessageDebug("Start Build ${skywalker_script_name}")

# 导出宏
macro(SkywalkerScript_Export TargetName)
    SkywalkerServer_Export(${TargetName} ${skywalker_script_name})
endmacro()

# SkywalkerTools 路径
set(skywalker_tools_path "../SkywalkerTools")

include_directories(
    ${skywalker_tools_path}
    ./
)

add_executable(${skywalker_script_name}
    SkywalkerScript.cpp
)

SkywalkerScript_Export(${skywalker_script_name})

SkywalkerServer_CopyToTarget(${skywalker_script_name} ${skywalker_server_tool_dir})

SkywalkerScript_MessageDebug("End Build ${skywalker_script_name}")
