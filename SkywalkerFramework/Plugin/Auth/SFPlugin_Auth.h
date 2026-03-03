/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\SFPlugin_Auth.h
**作者: shyfan
**日期: 2026/03/02
**功能: 认证插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPLUGIN_AUTH_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPLUGIN_AUTH_H__

#include "Core/Plugin/SFPlugin.h"

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
    virtual void Install(SFObjectErrors &Errors) override;
    virtual void Uninstall(SFObjectErrors &Errors) override;

#pragma endregion SSFPlugin
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFPLUGIN_AUTH_H__
