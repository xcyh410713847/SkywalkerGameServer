/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SSFPlugin_Test.h
**作者: shyfan
**日期: 2023/08/21 19:54:39
**功能: 测试插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__

#include "Core/Plugin/SSFPlugin.h"

SF_NAMESPACE_BEGIN

class SSFPlugin_Test : public SFPlugin
{
public:
    SSFPlugin_Test(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Test() {};

#pragma region SSFPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SFObjectErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SFObjectErrors &Errors) override;

#pragma endregion SSFPlugin
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__
