# #########################################################################
# 文件: Src\Executable\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:39:50
# 功能:
# #########################################################################

# 设置 SkywalkerServerFramework 静态库库的名称
set(skywalker_server_framework_name "SkywalkerServerFramework")

file(GLOB skywalker_game_server_source_cpp
    *.cpp)

file(GLOB skywalker_game_server_source_h
    *.h)

add_executable(${skywalker_game_server_name}
    ${skywalker_game_server_source_cpp}
    ${skywalker_game_server_source_h}
)

# 关闭警告
target_compile_options(${skywalker_game_server_name} PRIVATE "/wd4819")

target_link_libraries(${skywalker_game_server_name} PRIVATE
    ${skywalker_server_framework_name}
)
