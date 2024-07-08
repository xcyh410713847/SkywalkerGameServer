/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\AI\SSFPlugin_AI.h
**作者: shyfan
**日期: 2023/10/07 20:20:37
**功能: AI插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_AI : public SSFPlugin
{
public:
    SSFPlugin_AI(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_AI(){};

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__
