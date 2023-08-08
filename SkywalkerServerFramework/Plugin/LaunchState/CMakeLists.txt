# #########################################################################
# 文件: SkywalkerServerFramework\Plugin\LaunchState\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:38:17
# 功能:
# #########################################################################

# 设置宏
add_definitions(
    -DSKYWALKER_SF_DYNAMIC_PLUGIN
)

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

target_link_libraries(${skywalker_server_plugin_name} PRIVATE
    SSFCore
)
