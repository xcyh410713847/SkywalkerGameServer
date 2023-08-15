# #########################################################################
# 文件: SkywalkerServerFramework\Core\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/08 18:12:02
# 功能:
# #########################################################################

set(skywalker_server_framework_core_name "SSFCore")

file(GLOB skywalker_server_framework_core_source_cpp
    Plugin/*.cpp
    DynamicLib/*.cpp
)

file(GLOB skywalker_server_framework_core_source_h
    Plugin/*.h
    DynamicLib/*.h
)

add_library(${skywalker_server_framework_core_name} STATIC
    ${skywalker_server_framework_core_source_cpp}
    ${skywalker_server_framework_core_source_h}
)

SkywalkerServerFramework_Export(${skywalker_server_framework_core_name})

# 关闭警告
target_compile_options(${skywalker_server_framework_core_name} PRIVATE "/wd4819")
