/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\SSFPlugin_Test.cpp
**作者: shyfan
**日期: 2023/08/21 19:55:34
**功能: 测试插件
*************************************************************************/

#include "SSFPlugin_Test.h"

#include "Include/SSFILog.h"

#include "SSFModule_TestOne.h"
#include "SSFModule_TestTwo.h"
#include "SSFModule_TestThree.h"

SSF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFPlugin_Test, LogLevel_Debug);

SSF_PLUGIN_EXPORT(SSFPlugin_Test)

#pragma region SSFOPlugin

void SSFPlugin_Test::Install(SSFModuleErrors &Errors)
{
    SKYWALKER_SF_REGISTER_MODULE(PluginManager, SSFPlugin_Test, SSFModule_TestOne)
    SKYWALKER_SF_REGISTER_MODULE(PluginManager, SSFPlugin_Test, SSFModule_TestTwo)
    SKYWALKER_SF_REGISTER_MODULE(PluginManager, SSFPlugin_Test, SSFModule_TestThree)
}

void SSFPlugin_Test::Uninstall(SSFModuleErrors &Errors)
{
    SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, SSFPlugin_Test, SSFModule_TestOne)
    SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, SSFPlugin_Test, SSFModule_TestTwo)
    SKYWALKER_SF_UNREGISTER_MODULE(PluginManager, SSFPlugin_Test, SSFModule_TestThree)
}

#pragma endregion SSFOPlugin
