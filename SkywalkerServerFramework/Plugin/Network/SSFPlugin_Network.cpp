/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\SSFPlugin_Network.cpp
**作者: shyfan
**日期: 2023/08/15 20:06:04
**功能: 网络插件
*************************************************************************/

#include "SSFPlugin_Network.h"

SKYWALKER_SF_NAMESPACE_USE

#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN

#pragma region 插件导出

SKYWALKER_SF_API void DllStartPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager)
{
    SKYWALKER_SF_REGISTER_PLUGIN(PluginManager, SSFCPlugin_Network);
}

SKYWALKER_SF_API void DllStopPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager)
{
    SKYWALKER_SF_UNREGISTER_PLUGIN(PluginManager, SSFCPlugin_Network);
}

#pragma endregion 插件导出

#endif // SKYWALKER_SF_DYNAMIC_PLUGIN

#pragma region Object

void SSFCPlugin_Network::Init(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Init
}

void SSFCPlugin_Network::Awake(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Awake
}

void SSFCPlugin_Network::Start(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Start
}

void SSFCPlugin_Network::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    // TODO Shyfan 插件管理器Tick
}

void SSFCPlugin_Network::Stop(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Stop
}

void SSFCPlugin_Network::Sleep(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Sleep
}

void SSFCPlugin_Network::Destroy(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Destroy
}

#pragma endregion Object
