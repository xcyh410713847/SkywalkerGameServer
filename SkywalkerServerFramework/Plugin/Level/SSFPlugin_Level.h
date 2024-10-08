﻿/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Level\SSFPlugin_Level.h
**作者: shyfan
**日期: 2023/09/26 20:51:02
**功能: 关卡插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_Level : public SSFPlugin
{
public:
    SSFPlugin_Level(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Level(){};
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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__
