# #########################################################################
# 文件: CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:43:06
# 功能:
# #########################################################################
# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

function(SkywalkerServer_MessageDebug info)
    message(STATUS "[Skywalker Server] ${info}")
endfunction()

# 设置项目名称
set(skywalker_server_name "SkywalkerServer")

# 项目名称
project(skywalker_server_name)

SkywalkerServer_MessageDebug("Start Build ${skywalker_server_name}")

# 设置导出路径
set(Skywalker_Server_output_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Bin CACHE STRING "Skywalker_Server_output_PATH")

# 导出路径
SkywalkerServer_MessageDebug("OutputPath: ${Skywalker_Server_output_PATH}")

# 编译完成后拷贝到指定目录
macro(SkywalkerServer_CopyToTarget TargetName ProjectName)
    add_custom_command(TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${TargetName}>
        ${Skywalker_Server_output_PATH}/${ProjectName}/$<TARGET_FILE_NAME:${TargetName}>
    )
endmacro()

# 导出宏
macro(SkywalkerServer_Export TargetName ProjectName)
    SkywalkerServer_MessageDebug("Export ${ProjectName} ${TargetName}")

    # 设置编译之后的输出路径
    set_target_properties(${TargetName} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${Skywalker_Server_output_PATH}/${ProjectName}
        ARCHIVE_OUTPUT_DIRECTORY ${Skywalker_Server_output_PATH}/${ProjectName}
        RUNTIME_OUTPUT_DIRECTORY ${Skywalker_Server_output_PATH}/${ProjectName}
    )

    # SkywalkerServer_CopyToTarget(${TargetName} ${ProjectName})
endmacro()

# SkywalkerServerFramework
add_subdirectory(SkywalkerServerFramework)

# SkywalkerServerEditor
add_subdirectory(SkywalkerServerEditor)

# SkywalkerGameServer
add_subdirectory(SkywalkerGameServer)

# SkywalkerGameClient
add_subdirectory(SkywalkerGameClient)

SkywalkerServer_MessageDebug("End Build ${skywalker_server_name}")
