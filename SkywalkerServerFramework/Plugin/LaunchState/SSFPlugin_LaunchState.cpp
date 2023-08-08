/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\LaunchState\SSFPlugin_LaunchState.cpp
**作者: shyfan
**日期: 2023/07/30 12:38:27
**功能: 启动状态插件
*************************************************************************/

#include "SSFPlugin_LaunchState.h"

SKYWALKER_SF_NAMESPACE_USE

#pragma region 插件导出

SKYWALKER_SF_API void DllStartPlugin(SSFSharedPtr_IPluginManager PluginManager)
{
    SSFPluginErrors Errors;
    SKYWALKER_SF_CREATE_PLUGIN(PluginManager, SSFCPlugin_LaunchState, Errors)
}

SKYWALKER_SF_API void DllStopPlugin(SSFSharedPtr_IPluginManager PluginManager)
{
    SSFPluginErrors Errors;
    SKYWALKER_SF_DESTROY_PLUGIN(PluginManager, SSFCPlugin_LaunchState, Errors)
}

#pragma endregion 插件导出

SSFCPlugin_LaunchState::SSFCPlugin_LaunchState(SSFSharedPtr_IPluginManager InPluginManager)
{
    PluginManager = InPluginManager;
}

SSFCPlugin_LaunchState::~SSFCPlugin_LaunchState()
{
}

#pragma region SSFIPlugin

const std::string SSFCPlugin_LaunchState::GetPluginName()
{
    return SKYWALKER_SF_CLASS_NAME(SSFCPlugin_LaunchState);
}

#pragma endregion SSFIPlugin
