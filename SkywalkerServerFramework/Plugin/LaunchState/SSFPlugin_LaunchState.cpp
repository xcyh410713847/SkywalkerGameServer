/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\LaunchState\SSFPlugin_LaunchState.cpp
**作者: shyfan
**日期: 2023/07/30 12:38:27
**功能: 启动状态插件
*************************************************************************/

#include "SSFPlugin_LaunchState.h"

SKYWALKER_SF_NAMESPACE_USE

#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN

#pragma region 插件导出

SKYWALKER_SF_API void DllStartPlugin(SSFPtr_IPluginManager PluginManager)
{
    SKYWALKER_SF_CREATE_PLUGIN(PluginManager, SSFCPlugin_LaunchState);
}

SKYWALKER_SF_API void DllStopPlugin(SSFPtr_IPluginManager PluginManager)
{
    SKYWALKER_SF_DESTROY_PLUGIN(PluginManager, SSFCPlugin_LaunchState);
}

#pragma endregion 插件导出

#endif // SKYWALKER_SF_DYNAMIC_PLUGIN

SSFCPlugin_LaunchState::SSFCPlugin_LaunchState(SSFPtr_IPluginManager InPluginManager)
{
    PluginManager = InPluginManager;
}

SSFCPlugin_LaunchState::~SSFCPlugin_LaunchState()
{
}

#pragma region Object Base Interface

void SSFCPlugin_LaunchState::Init(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Init
}

void SSFCPlugin_LaunchState::Awake(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Awake
}

void SSFCPlugin_LaunchState::Start(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Start
}

void SSFCPlugin_LaunchState::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    // TODO Shyfan 插件管理器Tick
    std::cout << "SSFCPlugin_LaunchState::Tick" << std::endl;
}

void SSFCPlugin_LaunchState::Stop(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Stop
}

void SSFCPlugin_LaunchState::Sleep(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Sleep
}

void SSFCPlugin_LaunchState::Destroy(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Destroy
}

#pragma endregion Object Base Interface

#pragma region SSFIPlugin

const std::string SSFCPlugin_LaunchState::GetPluginName()
{
    return SKYWALKER_SF_CLASS_NAME(SSFCPlugin_LaunchState);
}

#pragma endregion SSFIPlugin
