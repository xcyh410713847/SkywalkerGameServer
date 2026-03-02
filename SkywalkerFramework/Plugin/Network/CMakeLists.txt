# #########################################################################
# 文件: SkywalkerFramework\Plugin\Network\CMakeLists.txt
# 作者: shyfan
# 日期: 2023/08/15 20:02:22
# 功能: 插件 Network 的 CMakeLists.txt
# #########################################################################

# 构建 Network 插件动态库
# 包含 Module/Object/Protocol/Router/Session 目录下源码（由 SkywalkerServer_Source 扫描）
SkywalkerServer_Plugin("SSFPlugin_Network" ${skywalker_server_framework_name})
