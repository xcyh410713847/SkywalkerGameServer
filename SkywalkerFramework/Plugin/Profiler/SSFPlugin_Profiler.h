/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\SSFPlugin_Profiler.h
**作者: shyfan
**日期: 2023/09/25 20:20:26
**功能: 分析器插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__

#include "Core/Plugin/SSFPlugin.h"

SF_NAMESPACE_BEGIN

class SSFPlugin_Profiler : public SFPlugin
{
public:
    SSFPlugin_Profiler(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Profiler() {};

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

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__
