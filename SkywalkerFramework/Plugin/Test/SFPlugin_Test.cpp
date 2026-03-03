/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SFPlugin_Test.cpp
**作者: shyfan
**日期: 2023/08/21 19:55:34
**功能: 测试插件
*************************************************************************/

#include "SFPlugin_Test.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"

#include "SFModule_TestOne.h"
#include "SFModule_TestTwo.h"
#include "SFModule_TestThree.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFPlugin_Test, ESFLogLevel::Debug);

SF_PLUGIN_EXPORT(SFPlugin_Test)

#pragma region SSFPlugin

void SFPlugin_Test::Install(SFObjectErrors &Errors)
{
    SF_REGISTER_MODULE(SFModule_TestOne)
    SF_REGISTER_MODULE(SFModule_TestTwo)
    SF_REGISTER_MODULE(SFModule_TestThree)
}

void SFPlugin_Test::Uninstall(SFObjectErrors &Errors)
{
    SF_UNREGISTER_MODULE(SFModule_TestOne)
    SF_UNREGISTER_MODULE(SFModule_TestTwo)
    SF_UNREGISTER_MODULE(SFModule_TestThree)
}

#pragma endregion SSFPlugin
