/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SFPlugin_Test.h
**作者: shyfan
**日期: 2023/08/21 19:54:39
**功能: 测试插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_TEST_H__

#include "Core/Plugin/SFPlugin.h"

SF_NAMESPACE_BEGIN

class SFPlugin_Test : public SFPlugin
{
public:
    SFPlugin_Test(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_Test() {};

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
