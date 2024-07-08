/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Profiler\SSFPlugin_Profiler.h
**作者: shyfan
**日期: 2023/09/25 20:20:26
**功能: 分析器插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_Profiler : public SSFPlugin
{
public:
    SSFPlugin_Profiler(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Profiler(){};

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

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__
