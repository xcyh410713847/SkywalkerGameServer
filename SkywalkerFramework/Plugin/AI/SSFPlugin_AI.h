/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\SSFPlugin_AI.h
**作者: shyfan
**日期: 2023/10/07 20:20:37
**功能: AI插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__

#include "Core/Plugin/SSFPlugin.h"

SF_NAMESPACE_BEGIN

class SSFPlugin_AI : public SFPlugin
{
public:
    SSFPlugin_AI(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_AI() {};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__
