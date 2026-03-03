# #########################################################################
# 文件: SkywalkerTest\CMakeLists.txt
# 作者: shyfan
# 日期: 2026/03/03
# 功能: Skywalker 测试工程 CMakeLists.txt
# #########################################################################

# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

# 输出日志级别
set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerTest_MessageDebug info)
    message(STATUS "[Skywalker Test] ${info}")
endfunction()

# 设置项目名称
set(skywalker_test_name "SkywalkerTest")

# 项目名称
project(${skywalker_test_name})

SkywalkerTest_MessageDebug("Start Build ${skywalker_test_name}")

# 导出宏
macro(SkywalkerTest_Export TargetName)
    SkywalkerServer_Export(${TargetName} ${skywalker_test_name})
endmacro()

# 包含路径
set(skywalker_server_path "../")
set(skywalker_tools_path "../SkywalkerTools")
set(skywalker_server_framework_path "../SkywalkerFramework")

include_directories(
    ${skywalker_server_path}
    ${skywalker_tools_path}
    ${skywalker_server_framework_path}
)

# 创建测试执行文件
add_executable(SkywalkerTestRunner
    SkywalkerTestMain.cpp
    SkywalkerNetworkTest.cpp
    ../SkywalkerFramework/Plugin/Network/Protocol/SFNetworkCodec.cpp
    ../SkywalkerFramework/Plugin/Network/Protocol/SFNetworkLoginPayload.cpp
    ../SkywalkerFramework/Plugin/Network/Router/SFNetworkRouter.cpp
    ../SkywalkerFramework/Plugin/Network/Session/SFNetworkSessionManager.cpp
    ../SkywalkerFramework/Plugin/Network/Session/SFNetworkBlacklistStore.cpp
    ../SkywalkerFramework/Plugin/Level/Module/SFModule_WorldManager.cpp
    ../SkywalkerFramework/Plugin/CommandLine/Module/SFModule_AdminCommand.cpp
    ../SkywalkerFramework/Plugin/AI/Module/SFModule_AIRuntime.cpp
    ../SkywalkerFramework/Plugin/Video/Module/SFModule_ReplayRecorder.cpp
    ../SkywalkerFramework/Plugin/Video/Module/SFModule_ReplayPlayer.cpp
)

target_link_libraries(SkywalkerTestRunner PRIVATE
    ${skywalker_server_framework_name}
    SFCore
)

SkywalkerTest_Export(SkywalkerTestRunner)

SkywalkerServer_CopyToTarget(SkywalkerTestRunner ${skywalker_server_server_dir})

SkywalkerTest_MessageDebug("End Build ${skywalker_test_name}")
