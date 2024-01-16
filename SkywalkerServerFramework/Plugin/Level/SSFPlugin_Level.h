/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Level\SSFPlugin_Level.h
**作者: shyfan
**日期: 2023/09/26 20:51:02
**功能: 关卡插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_Level : public SSFOPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_Level)

public:
    SSFPlugin_Level(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFOPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Level(){};
#pragma region SSFOPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SSFObjectErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SSFObjectErrors &Errors) override;

#pragma endregion SSFOPlugin
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__
