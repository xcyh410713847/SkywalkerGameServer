/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\SFPlugin_Auth.h
**作者: shyfan
**日期: 2026/03/19
**功能: 登录鉴权插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_AUTH_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_AUTH_H__

#include "Core/Plugin/SFPlugin.h"

#pragma comment(lib, "ws2_32.lib")

SF_NAMESPACE_BEGIN

class SFPlugin_Auth : public SFPlugin
{
public:
    SFPlugin_Auth(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_Auth() {};

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

#endif /* __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_AUTH_H__ */
