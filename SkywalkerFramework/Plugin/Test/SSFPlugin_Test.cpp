/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SSFPlugin_Test.cpp
**作者: shyfan
**日期: 2023/08/21 19:55:34
**功能: 测试插件
*************************************************************************/

#include "SSFPlugin_Test.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SSFPluginManager.h"

#include "SSFModule_TestOne.h"
#include "SSFModule_TestTwo.h"
#include "SSFModule_TestThree.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFPlugin_Test, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SSFPlugin_Test)

#pragma region SSFPlugin

void SSFPlugin_Test::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SSFModule_TestOne)
    SF_REGISTER_MODULE(SSFModule_TestTwo)
    SF_REGISTER_MODULE(SSFModule_TestThree)
}

void SSFPlugin_Test::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SSFModule_TestOne)
    SF_UNREGISTER_MODULE(SSFModule_TestTwo)
    SF_UNREGISTER_MODULE(SSFModule_TestThree)
}

#pragma endregion SSFPlugin
