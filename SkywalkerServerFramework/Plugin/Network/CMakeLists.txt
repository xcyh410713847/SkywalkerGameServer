# #########################################################################
# 文件: SkywalkerServerFramework\Plugin\Network\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/15 20:02:22
# 功能: 插件 Network 的 CMakeLists.txt
# #########################################################################

# 设置宏
add_definitions(
    -DSKYWALKER_SF_DYNAMIC_PLUGIN
)

set(skywalker_server_plugin_name "SSFPlugin_Network")

file(GLOB skywalker_server_plugin_source_cpp
    *.cpp)

file(GLOB skywalker_server_plugin_source_h
    *.h)

add_library(${skywalker_server_plugin_name} SHARED
    ${skywalker_server_plugin_source_cpp}
    ${skywalker_server_plugin_source_h}
)

target_link_libraries(${skywalker_server_plugin_name} PRIVATE
    SSFCore
)

SkywalkerServerFramework_Export(${skywalker_server_plugin_name})

SkywalkerServer_CopyToTarget(${skywalker_server_plugin_name} ${skywalker_server_server_DIR})

# 关闭警告
target_compile_options(${skywalker_server_plugin_name} PRIVATE "/wd4819")
