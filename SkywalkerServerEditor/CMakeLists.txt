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

# 设置项目名称
set(skywalker_server_editor_name "SkywalkerServerEditor")

# 项目名称
project(${skywalker_server_editor_name})

SkywalkerServerEditor_MessageDebug("Start Build ${skywalker_server_editor_name}")

# 导出宏
macro(SkywalkerGameEditor_Export TargetName)
    SkywalkerServer_Export(${TargetName} ${skywalker_server_editor_name})
endmacro()

# SkywalkerTools 路径
set(skywalker_tools_path "../SkywalkerTools")

include_directories(
    ${skywalker_tools_path}
    ./
)

# 生成可执行文件
file(GLOB skywalker_server_editor_source_cpp
    *.cpp)

file(GLOB skywalker_server_editor_source_h
    *.h)

add_executable(${skywalker_server_editor_name}
    SkywalkerServerEditor.cpp
)

SkywalkerGameEditor_Export(${skywalker_server_editor_name})

# 关闭警告
target_compile_options(${skywalker_server_editor_name} PRIVATE "/wd4819")

SkywalkerServerEditor_MessageDebug("End Build ${skywalker_server_editor_name}")
