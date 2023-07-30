# #########################################################################
# 文件: Src\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/07/30 12:42:34
# 功能:
# #########################################################################

SkywalkerGameServer_MessageDebug("Start Build SkywalkerGameServer")

# 头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR} # 项目根目录
    ${CMAKE_CURRENT_SOURCE_DIR} # 当前CMakeLists.txt所在目录
)

# 服务器模块文件夹
add_subdirectory(Module)

# 执行文件文件夹
add_subdirectory(Executable)

SkywalkerGameServer_MessageDebug("End Build SkywalkerGameServer")
