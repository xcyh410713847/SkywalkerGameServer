/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\SSFPlugin_Network.cpp
**作者: shyfan
**日期: 2023/08/15 20:06:04
**功能: 网络插件
*************************************************************************/

#include "SSFPlugin_Network.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_Network, LogLevel_Debug);

#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN

#define SKYWALKER_SF_PLUGIN_CLASS SSFOPlugin_Network

#pragma region 插件导出

SKYWALKER_SF_API void DllStartPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager)
{
    SKYWALKER_SF_REGISTER_PLUGIN(PluginManager, SKYWALKER_SF_PLUGIN_CLASS);
}

SKYWALKER_SF_API void DllStopPlugin(SKYWALKER_SF_PTR_PLUGIN_MANAGER PluginManager)
{
    SKYWALKER_SF_UNREGISTER_PLUGIN(PluginManager, SKYWALKER_SF_PLUGIN_CLASS);
}

#pragma endregion 插件导出

#endif // SKYWALKER_SF_DYNAMIC_PLUGIN
