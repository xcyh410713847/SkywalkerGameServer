set(skywalker_game_server_module_name "Launcher")

file(GLOB skywalker_game_server_source_cpp
    *.cpp)

file(GLOB skywalker_game_server_source_h
    *.h)

add_library(${skywalker_game_server_module_name} SHARED
    ${skywalker_game_server_source_cpp}
    ${skywalker_game_server_source_h}
)

# 关闭警告
target_compile_options(${skywalker_game_server_module_name} PRIVATE "/wd4819")
