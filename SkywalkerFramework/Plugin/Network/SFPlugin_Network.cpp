/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\SFPlugin_Network.cpp
**作者: shyfan
**日期: 2023/08/15 20:06:04
**功能: 网络插件
*************************************************************************/
#include "SFPlugin_Network.h"

#include "SFPlugin_Network.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

#include "SFModule_NetworkServer.h"
#include "SFModule_NetworkClient.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Network, ESFLogLevel::Debug);

// 插件导出
SF_PLUGIN_EXPORT(SFPlugin_Network)

#pragma region SSFPlugin

void SFPlugin_Network::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SFModule_NetworkServer)
    SF_REGISTER_MODULE(SFModule_NetworkClient)
}

void SFPlugin_Network::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SFModule_NetworkServer)
    SF_UNREGISTER_MODULE(SFModule_NetworkClient)
}

#pragma endregion SSFPlugin
