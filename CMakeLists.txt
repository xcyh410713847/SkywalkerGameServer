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
project(${skywalker_server_name})

SkywalkerServer_MessageDebug("Start Build ${skywalker_server_name}")

# 导出相关
set(skywalker_server_output_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Bin CACHE STRING "skywalker_server_output_PATH")
set(skywalker_server_server_DIR "Server" CACHE STRING "skywalker_server_server_DIR")
set(skywalker_server_editor_DIR "Editor" CACHE STRING "skywalker_server_editor_DIR")
set(skywalker_server_client_DIR "Client" CACHE STRING "skywalker_server_client_DIR")

# 编译完成后拷贝到指定目录
macro(SkywalkerServer_CopyToTarget TargetName DirName)
    SkywalkerServer_MessageDebug("Copy ${TargetName} To ${skywalker_server_output_PATH}/${DirName} ")

    add_custom_command(TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${TargetName}>
        ${skywalker_server_output_PATH}/${DirName}/$<TARGET_FILE_NAME:${TargetName}>
    )
endmacro()

# 导出宏
macro(SkywalkerServer_Export TargetName ProjectName)
    SkywalkerServer_MessageDebug("Export ${TargetName} To ${skywalker_server_output_PATH}/${ProjectName} ")

    # 设置编译之后的输出路径
    set_target_properties(${TargetName} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${skywalker_server_output_PATH}/${ProjectName}
        ARCHIVE_OUTPUT_DIRECTORY ${skywalker_server_output_PATH}/${ProjectName}
        RUNTIME_OUTPUT_DIRECTORY ${skywalker_server_output_PATH}/${ProjectName}
    )
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
