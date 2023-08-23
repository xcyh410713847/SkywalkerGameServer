# #########################################################################
# 文件: SkywalkerServerFramework\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:39:03
# 功能: Skywalker Server Framework 项目CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerServerFramework_MessageDebug info)
    message(STATUS "[Skywalker Server Framework] ${info}")
endfunction()

# 设置项目名称
set(skywalker_server_framework_name "SkywalkerServerFramework" CACHE STRING "Skywalker Server Framework 项目名称")

# 项目名称
project(${skywalker_server_framework_name})

SkywalkerServerFramework_MessageDebug(" Start Build ${skywalker_server_framework_name} ")

# 导出宏
macro(SkywalkerServerFramework_Export TargetName)
    SkywalkerServer_Export(${TargetName} ${skywalker_server_framework_name})
endmacro()

# SkywalkerTools 路径
set(skywalker_tools_path " ../SkywalkerTools ")

include_directories(
    ${skywalker_tools_path}
    ./
)

# 核心
add_subdirectory(Core)

# 插件
add_subdirectory(Plugin)

add_library(${skywalker_server_framework_name} STATIC
    SkywalkerServerFramework.cpp
    SkywalkerServerFramework.h
)

target_link_libraries(${skywalker_server_framework_name} PRIVATE
    ${skywalker_server_framework_core_name}
)

SkywalkerServerFramework_Export(${skywalker_server_framework_name})

SkywalkerServer_CopyToTarget(${skywalker_server_framework_name} ${skywalker_server_server_dir})

SkywalkerServerFramework_MessageDebug(" End Build ${skywalker_server_framework_name} ")