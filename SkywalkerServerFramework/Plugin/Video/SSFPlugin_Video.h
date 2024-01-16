/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Video\SSFPlugin_Video.h
**作者: shyfan
**日期: 2023/09/25 23:01:57
**功能: 录像插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_Video : public SSFOPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_Video)

public:
    SSFPlugin_Video(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFOPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Video(){};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__
