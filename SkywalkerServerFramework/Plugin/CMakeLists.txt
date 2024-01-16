# #########################################################################
# 文件: SkywalkerServerFramework\Plugin\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:38:52
# 功能:
# #########################################################################

# 获取当前目录下所有的文件夹名称
file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

SkywalkerServerFramework_MessageDebug(" Start Load ${skywalker_server_framework_name} Plugin")

# 遍历所有的文件夹
foreach(subdirectory ${subdirectories})
    # 如果是文件夹则添加子目录
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory})
        add_subdirectory(${subdirectory})
    endif()
endforeach()

SkywalkerServerFramework_MessageDebug(" End Load ${skywalker_server_framework_name} Plugin")
