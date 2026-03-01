# #########################################################################
# 文件: SkywalkerFramework\Plugin\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:38:52
# 功能:
# #########################################################################

# 获取当前目录下所有子项名称
file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

SkywalkerServerFramework_MessageDebug(" Start Load ${skywalker_server_framework_name} Plugin")

# 遍历并加载插件子目录
foreach(subdirectory ${subdirectories})
    # 仅对子目录执行 add_subdirectory
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory})
        add_subdirectory(${subdirectory})
    endif()
endforeach()

SkywalkerServerFramework_MessageDebug(" End Load ${skywalker_server_framework_name} Plugin")
