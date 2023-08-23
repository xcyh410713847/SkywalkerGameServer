# #########################################################################
# 文件: SkywalkerServerFramework\Plugin\Test\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/21 19:53:46
# 功能: 测试插件 CMakeLists.txt
# #########################################################################

# 设置宏
add_definitions(
    -DSKYWALKER_SF_DYNAMIC_PLUGIN
)

set(skywalker_server_plugin_name "SSFPlugin_Test")

include_directories(
    ./Public
)

SkywalkerServer_Source()

add_library(${skywalker_server_plugin_name} SHARED
    ${skywalker_server_source}
)

target_link_libraries(${skywalker_server_plugin_name} PRIVATE
    SSFCore
)

SkywalkerServerFramework_Export(${skywalker_server_plugin_name})

SkywalkerServer_CopyToTarget(${skywalker_server_plugin_name} ${skywalker_server_server_dir})
