# #########################################################################
# 文件: SkywalkerServerEditor\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/03 10:34:42
# 功能: Skywalker Server Editor 项目CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerServerEditor_MessageDebug info)
    message(STATUS "[Skywalker Server Editor] ${info}")
endfunction()

SkywalkerServerEditor_MessageDebug("Start Build Skywalker Server Editor")

# 设置项目名称
set(skywalker_server_editor_name "SkywalkerServerEditor")

# 项目名称
project(${skywalker_server_editor_name})

# SkywalkerTools 路径
set(skywalker_tools_path "../SkywalkerTools")

include_directories(
    ${skywalker_tools_path}
    ./
)

add_subdirectory(Editor)

SkywalkerServerEditor_MessageDebug("End Build Skywalker Server Editor")