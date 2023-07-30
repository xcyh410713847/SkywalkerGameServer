# #########################################################################
# 文件: SkywalkerServerFramework\Core\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:34:49
# 功能:
# #########################################################################

set(skywalker_server_framework_core_name "SSFCore")

file(GLOB skywalker_server_framework_source_cpp
    *.cpp)

file(GLOB skywalker_server_framework_source_h
    *.h)

add_library(${skywalker_server_framework_core_name} STATIC
    ${skywalker_server_framework_source_cpp}
    ${skywalker_server_framework_source_h}
)

# 关闭警告
target_compile_options(${skywalker_server_framework_core_name} PRIVATE "/wd4819")
