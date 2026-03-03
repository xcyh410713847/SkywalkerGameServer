/*************************************************************************
**文件: SkywalkerFramework\Plugin\Actor\SFPlugin_Actor.h
**作者: shyfan
**日期: 2023/09/28 10:14:54
**功能: Actor插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Actor_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Actor_H__

#include "Core/Plugin/SFPlugin.h"

SF_NAMESPACE_BEGIN

class SFPlugin_Actor : public SFPlugin
{
public:
    SFPlugin_Actor(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_Actor() {};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Actor_H__
