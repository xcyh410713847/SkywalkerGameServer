# #########################################################################
# 文件: CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:43:06
# 功能:
# #########################################################################
# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.12)

set(CMAKE_MESSAGE_LOG_LEVEL "STATUS")

# 支持C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 必须使用C++17

function(SkywalkerServer_MessageDebug info)
    message(STATUS "[Skywalker Server] ${info}")
endfunction()

# 设置项目名称
set(skywalker_server_name "SkywalkerServer")

# 项目名称
project(${skywalker_server_name})

SkywalkerServer_MessageDebug("Start Build ${skywalker_server_name}")

# 项目模式
set(skywalker_server_mode_Project_Release "ProjectReleas" CACHE STRING "skywalker_server_mode_Project_Release")
set(skywalker_server_mode_Project_Debug "ProjectDebug" CACHE STRING "skywalker_server_mode_Project_Debug")
set(skywalker_server_mode_Library "Library" CACHE STRING "skywalker_server_mode_Library")

# 设置项目模式
set(skywalker_server_MODE ${skywalker_server_mode_Project_Debug} CACHE STRING "skywalker_server_MODE")

# 导出相关
set(skywalker_server_output_path ${CMAKE_CURRENT_SOURCE_DIR}/Bin CACHE STRING "skywalker_server_output_path")
set(skywalker_server_server_dir "Server" CACHE STRING "skywalker_server_server_dir")
set(skywalker_server_editor_dir "Editor" CACHE STRING "skywalker_server_editor_dir")
set(skywalker_server_client_dir "Client" CACHE STRING "skywalker_server_client_dir")

# 编译完成后拷贝到指定目录
macro(SkywalkerServer_CopyToTarget TargetName DirName)
    if(${skywalker_server_MODE} MATCHES ${skywalker_server_mode_Project_Release})
        SkywalkerServer_MessageDebug("Copy ${TargetName} To ${skywalker_server_output_path}/${DirName}")

        add_custom_command(TARGET ${TargetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:${TargetName}>
            ${skywalker_server_output_path}/${DirName}/$<TARGET_FILE_NAME:${TargetName}>
        )
    endif()
endmacro()

# 导出宏
macro(SkywalkerServer_Export TargetName ProjectName)
    if(UNIX)
        set_target_properties(${TargetName} PROPERTIES PREFIX "")
    endif()

    if(${skywalker_server_MODE} MATCHES ${skywalker_server_mode_Project_Debug})
        SkywalkerServer_MessageDebug("Export ${TargetName} To ${skywalker_server_output_path}")

        set_target_properties(${TargetName} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${skywalker_server_output_path}
            ARCHIVE_OUTPUT_DIRECTORY ${skywalker_server_output_path}
            RUNTIME_OUTPUT_DIRECTORY ${skywalker_server_output_path}
        )
    else()
        SkywalkerServer_MessageDebug("Export ${TargetName} To ${skywalker_server_output_path}/${ProjectName}")

        set_target_properties(${TargetName} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${skywalker_server_output_path}/${ProjectName}
            ARCHIVE_OUTPUT_DIRECTORY ${skywalker_server_output_path}/${ProjectName}
            RUNTIME_OUTPUT_DIRECTORY ${skywalker_server_output_path}/${ProjectName}
        )
    endif()
endmacro()

# 源码宏
macro(SkywalkerServer_Source)
    file(GLOB skywalker_server_source

        # cpp
        *.cpp
        */*.cpp

        # h
        *.h
        */*.h
    )
endmacro()

# 插件宏
macro(SkywalkerServer_Plugin PlugName DirName)
    add_definitions(
        -DSKYWALKER_SF_DYNAMIC_PLUGIN
    )

    include_directories(
        ./Public
        ./Private
    )

    SkywalkerServer_Source()

    add_library(${PlugName} SHARED
        ${skywalker_server_source}
    )

    target_link_libraries(${PlugName} PRIVATE
        SSFCore
    )

    SkywalkerServer_Export(${PlugName} ${DirName})

    SkywalkerServer_CopyToTarget(${PlugName} ${skywalker_server_server_dir})
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
