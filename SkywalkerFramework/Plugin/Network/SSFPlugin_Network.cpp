/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\SSFPlugin_Network.cpp
**作者: shyfan
**日期: 2023/08/15 20:06:04
**功能: 网络插件
*************************************************************************/

#include "SSFPlugin_Network.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

#include "SSFModule_NetworkServer.h"
#include "SSFModule_NetworkClient.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_Network, ESFLogLevel::Debug);

// 插件导出
SF_PLUGIN_EXPORT(SSFPlugin_Network)

#pragma region SSFPlugin

void SSFPlugin_Network::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SSFModule_NetworkServer)
    SF_REGISTER_MODULE(SSFModule_NetworkClient)
}

void SSFPlugin_Network::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SSFModule_NetworkServer)
    SF_UNREGISTER_MODULE(SSFModule_NetworkClient)
}

#pragma endregion SSFPlugin
