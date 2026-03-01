# #########################################################################
# 文件: SkywalkerFramework\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:39:03
# 功能: Skywalker Framework 项目CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

# 输出日志级别
set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerServerFramework_MessageDebug info)
    message(STATUS "[Skywalker Framework] ${info}")
endfunction()

# 设置项目名称
set(skywalker_server_framework_name "SkywalkerFramework" CACHE STRING "Skywalker Framework 项目名称")

# 项目名称
project(${skywalker_server_framework_name})

SkywalkerServerFramework_MessageDebug(" Start Build ${skywalker_server_framework_name} ")

# 核心库名称（清理历史缓存，避免仍引用旧名 SSFCore）
unset(skywalker_server_framework_core_name CACHE)
set(skywalker_server_framework_core_name "SFCore")

# ==============================
# 目录与依赖配置
# ==============================

# 导出宏
macro(SkywalkerServerFramework_Export TargetName)
    SkywalkerServer_Export(${TargetName} ${skywalker_server_framework_name})
endmacro()

# SkywalkerServer 路径
set(skywalker_server_path "../")

# SkywalkerTools 路径
set(skywalker_tools_path "../SkywalkerTools")

include_directories(
    ${skywalker_server_path}
    ${skywalker_tools_path}
    ./
)

# 核心
add_subdirectory(Core)

# 插件
add_subdirectory(Plugin)

# ==============================
# Framework 聚合静态库
# ==============================

add_library(${skywalker_server_framework_name} STATIC
    SkywalkerFramework.cpp
    SkywalkerFramework.h
)

target_link_libraries(${skywalker_server_framework_name} PRIVATE
    ${skywalker_server_framework_core_name}
)

SkywalkerServerFramework_Export(${skywalker_server_framework_name})

SkywalkerServer_CopyToTarget(${skywalker_server_framework_name} ${skywalker_server_server_dir})

SkywalkerServerFramework_MessageDebug(" End Build ${skywalker_server_framework_name} ")