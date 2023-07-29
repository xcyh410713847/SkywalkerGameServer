set(skywalker_game_server_exe_name "SkywalkerServer")

file(GLOB skywalker_game_server_source_cpp
    *.cpp)

file(GLOB skywalker_game_server_source_h
    *.h)

add_executable(${skywalker_game_server_exe_name}
    ${skywalker_game_server_source_cpp}
    ${skywalker_game_server_source_h}
)

# 关闭警告
target_compile_options(${skywalker_game_server_exe_name} PRIVATE "/wd4819")
