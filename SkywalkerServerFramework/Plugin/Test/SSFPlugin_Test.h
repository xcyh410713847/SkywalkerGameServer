﻿/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\SSFPlugin_Test.h
**作者: shyfan
**日期: 2023/08/21 19:54:39
**功能: 测试插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Plugin/SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFPlugin_Test : public SSFOPlugin
{
#pragma region Object
#pragma endregion Object

public:
    SSFPlugin_Test(SSFObjectCreatorContext &Context, SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOPlugin(Context, InPluginManager)
    {
    }
    virtual ~SSFPlugin_Test(){};

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFPlugin_Test);
    };

#pragma region SSFOPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SSFModuleErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SSFModuleErrors &Errors) override;

#pragma endregion SSFOPlugin
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__
