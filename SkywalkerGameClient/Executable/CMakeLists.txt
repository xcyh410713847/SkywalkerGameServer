# #########################################################################
# 文件: SkywalkerGameClient\Executable\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/07 11:33:08
# 功能:
# #########################################################################

file(GLOB skywalker_game_client_source_cpp
    *.cpp)

file(GLOB skywalker_game_client_source_h
    *.h)

add_executable(${skywalker_game_client_name}
    ${skywalker_game_client_source_cpp}
    ${skywalker_game_client_source_h}
)

# 关闭警告
target_compile_options(${skywalker_game_client_name} PRIVATE "/wd4819")
