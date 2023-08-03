# #########################################################################
# 文件: SkywalkerServerEditor\Editor\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/03 11:25:39
# 功能: 编辑器
# #########################################################################

file(GLOB skywalker_server_editor_source_cpp
    *.cpp)

file(GLOB skywalker_server_editor_source_h
    *.h)

add_executable(${skywalker_server_editor_name}
    ${skywalker_server_editor_source_cpp}
    ${skywalker_server_editor_source_h}
)

# 关闭警告
target_compile_options(${skywalker_server_editor_name} PRIVATE "/wd4819")
