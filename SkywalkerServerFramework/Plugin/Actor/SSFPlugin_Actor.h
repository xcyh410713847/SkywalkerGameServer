/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Actor\SSFPlugin_Actor.h
**作者: shyfan
**日期: 2023/09/28 10:14:54
**功能: Actor插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Actor_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Actor_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_Actor : public SSFPlugin
{
public:
    SSFPlugin_Actor(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Actor(){};

#pragma region SSFPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SSFObjectErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SSFObjectErrors &Errors) override;

#pragma endregion SSFPlugin
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Actor_H__
