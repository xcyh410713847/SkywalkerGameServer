# #########################################################################
# 文件: SkywalkerServerFramework\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:39:03
# 功能:
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

function(SkywalkerServerFramework_MessageDebug info)
    message(STATUS "[Skywalker Server Framework] ${info}")
endfunction()

SkywalkerServerFramework_MessageDebug("Start Build Skywalker Server Framework")

# 设置项目名称
set(skywalker_server_framework_name "SkywalkerServerFramework")

# 项目名称
project(${skywalker_server_framework_name})

# SkywalkerTools 路径
set(skywalker_tools_path "../SkywalkerTools")

include_directories(
    ${skywalker_tools_path}
    ./
)

# 核心代码
add_subdirectory(Core)

# 插件
add_subdirectory(Plugin)

# 编辑器
add_subdirectory(Editor)

# 启动库
# add_subdirectory(Launcher)
SkywalkerServerFramework_MessageDebug("End Build Skywalker Server Framework")
