/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\SFPlugin_Level.h
**作者: shyfan
**日期: 2023/09/26 20:51:02
**功能: 关卡插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__

#include "Core/Plugin/SFPlugin.h"

SF_NAMESPACE_BEGIN

class SFPlugin_Level : public SFPlugin
{
public:
    SFPlugin_Level(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_Level() {};
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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Level_H__
