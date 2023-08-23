# #########################################################################
# 文件: SkywalkerServerFramework\Core\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/08 18:12:02
# 功能:
# #########################################################################

set(skywalker_server_framework_core_name "SSFCore" CACHE STRING "Skywalker Server Framework Core Library")

SkywalkerServer_Source()

add_library(${skywalker_server_framework_core_name} STATIC
    ${skywalker_server_source}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # 生成共享对象时没有使用 -fPIC 编译选项导致错误
    set_target_properties(${skywalker_server_framework_core_name} PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

SkywalkerServerFramework_Export(${skywalker_server_framework_core_name})

SkywalkerServer_CopyToTarget(${skywalker_server_framework_core_name} ${skywalker_server_server_dir})
