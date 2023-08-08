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
set(skywalker_server_framework_name "SkywalkerServerFramework")

# 项目名称
project(${skywalker_server_framework_name})

SkywalkerServerFramework_MessageDebug("Start Build Skywalker Server Framework")

# 支持C++17
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DSKYWALKER_SF_DYNAMIC_PLUGIN)

# SkywalkerTools 路径
set(skywalker_tools_path "../SkywalkerTools")

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
)

# 关闭警告
target_compile_options(${skywalker_server_framework_name} PRIVATE "/wd4819")

SkywalkerServerFramework_MessageDebug("End Build Skywalker Server Framework")
