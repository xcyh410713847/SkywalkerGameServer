/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\SSFPlugin_Test.cpp
**作者: shyfan
**日期: 2023/08/21 19:55:34
**功能: 测试插件
*************************************************************************/

#include "SSFPlugin_Test.h"

#include "SSFModule_PublicTest.h"

SKYWALKER_SF_NAMESPACE_USE

#ifdef SKYWALKER_SF_DYNAMIC_PLUGIN

#define SKYWALKER_SF_PLUGIN_CLASS SSFOPlugin_Test

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

#pragma region SSFOPlugin

void SSFOPlugin_Test::Install(SSFModuleErrors &Errors)
{
    SKYWALKER_SF_REGISTER_MODULE(PluginManager, SSFOPlugin_Test, SSFOModule_PublicTest)
}

void SSFOPlugin_Test::Uninstall(SSFModuleErrors &Errors)
{
    SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, SSFOPlugin_Test, SSFOModule_PublicTest)
}

#pragma endregion SSFOPlugin
