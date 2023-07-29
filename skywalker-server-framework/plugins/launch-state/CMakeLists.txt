set(skywalker_server_plugin_name "SSFPluginLaunchState")

file(GLOB skywalker_server_plugin_source_cpp
    *.cpp)

file(GLOB skywalker_server_plugin_source_h
    *.h)

add_library(${skywalker_server_plugin_name} SHARED
    ${skywalker_server_plugin_source_cpp}
    ${skywalker_server_plugin_source_h}
)

# 关闭警告
target_compile_options(${skywalker_server_plugin_name} PRIVATE "/wd4819")
