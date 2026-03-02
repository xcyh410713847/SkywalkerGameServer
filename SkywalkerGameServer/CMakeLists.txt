# #########################################################################
# 文件: Src\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:42:34
# 功能: Skywalker Game Server 项目CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

# 输出日志级别
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

# ==============================
# 头文件目录配置
# ==============================
# 包含路径
set(skywalker_server_path "../")
set(skywalker_tools_path "../SkywalkerTools")
set(skywalker_server_framework_path "../SkywalkerFramework")

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

# 测试程序链接 Framework 库

# 创建测试执行文件
add_executable(SkywalkerTestRunner
    SkywalkerTestMain.cpp
    SkywalkerNetworkTest.cpp
    ../SkywalkerFramework/Plugin/Network/Protocol/SSFNetworkCodec.cpp
    ../SkywalkerFramework/Plugin/Network/Protocol/SSFNetworkLoginPayload.cpp
    ../SkywalkerFramework/Plugin/Network/Router/SSFNetworkRouter.cpp
    ../SkywalkerFramework/Plugin/Network/Session/SSFNetworkSessionManager.cpp
    ../SkywalkerFramework/Plugin/Network/Session/SSFNetworkBlacklistStore.cpp
    ../SkywalkerFramework/Plugin/Level/Module/SSFModule_WorldManager.cpp
    ../SkywalkerFramework/Plugin/CommandLine/Module/SSFModule_AdminCommand.cpp
    ../SkywalkerFramework/Plugin/AI/Module/SSFModule_AIRuntime.cpp
    ../SkywalkerFramework/Plugin/Video/Module/SSFModule_ReplayRecorder.cpp
    ../SkywalkerFramework/Plugin/Video/Module/SSFModule_ReplayPlayer.cpp
)

target_link_libraries(SkywalkerTestRunner PRIVATE
    ${skywalker_server_framework_name}
    SFCore
)

SkywalkerGameServer_Export(SkywalkerTestRunner)

SkywalkerServer_CopyToTarget(SkywalkerTestRunner ${skywalker_server_server_dir})

target_link_libraries(${skywalker_game_server_name} PRIVATE
    ${skywalker_server_framework_name}
)

SkywalkerGameServer_Export(${skywalker_game_server_name})

SkywalkerServer_CopyToTarget(${skywalker_game_server_name} ${skywalker_server_server_dir})

SkywalkerGameServer_MessageDebug("End Build ${skywalker_game_server_name}")
