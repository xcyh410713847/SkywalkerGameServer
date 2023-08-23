/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\LaunchState\SSFPlugin_LaunchState.cpp
**作者: shyfan
**日期: 2023/07/30 12:38:27
**功能: 启动状态插件
*************************************************************************/

#include "SSFPlugin_LaunchState.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_LaunchState, LogLevel_Debug);

#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN

#define SKYWALKER_SF_PLUGIN_CLASS SSFOPlugin_LaunchState

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

#pragma region Object

void SSFOPlugin_LaunchState::Init(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Init
}

void SSFOPlugin_LaunchState::Awake(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Awake
}

void SSFOPlugin_LaunchState::Start(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Start
}

void SSFOPlugin_LaunchState::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    // TODO Shyfan 插件管理器Tick
}

void SSFOPlugin_LaunchState::Stop(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Stop
}

void SSFOPlugin_LaunchState::Sleep(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Sleep
}

void SSFOPlugin_LaunchState::Destroy(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Destroy
}

#pragma endregion Object
