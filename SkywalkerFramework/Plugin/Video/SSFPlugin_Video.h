/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\SSFPlugin_Video.h
**作者: shyfan
**日期: 2023/09/25 23:01:57
**功能: 录像插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__

#include "Core/Plugin/SSFPlugin.h"

SF_NAMESPACE_BEGIN

class SSFPlugin_Video : public SFPlugin
{
public:
    SSFPlugin_Video(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Video() {};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__
