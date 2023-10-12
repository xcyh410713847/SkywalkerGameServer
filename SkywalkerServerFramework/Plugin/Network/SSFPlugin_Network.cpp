/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\SSFPlugin_Network.cpp
**作者: shyfan
**日期: 2023/08/15 20:06:04
**功能: 网络插件
*************************************************************************/

#include "SSFPlugin_Network.h"

#include "Include/SSFILog.h"

#include "SSFModule_NetworkServer.h"
#include "SSFModule_NetworkClient.h"

SSF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_Network, LogLevel_Debug);

// 插件导出
SSF_PLUGIN_EXPORT(SSFPlugin_Network)

#pragma region SSFOPlugin

void SSFPlugin_Network::Install(SSFModuleErrors &Errors)
{
    SKYWALKER_SF_REGISTER_MODULE(PluginManager, SSFPlugin_Network, SSFModule_NetworkServer)
    SKYWALKER_SF_REGISTER_MODULE(PluginManager, SSFPlugin_Network, SSFModule_NetworkClient)
}

void SSFPlugin_Network::Uninstall(SSFModuleErrors &Errors)
{
    SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, SSFPlugin_Network, SSFModule_NetworkServer)
    SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, SSFPlugin_Network, SSFModule_NetworkClient)
}

#pragma endregion SSFOPlugin
