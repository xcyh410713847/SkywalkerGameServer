/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\SFPlugin_Profiler.h
**作者: shyfan
**日期: 2023/09/25 20:20:26
**功能: 分析器插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__

#pragma comment(lib, "ws2_32.lib")

#include "Core/Plugin/SFPlugin.h"

SF_NAMESPACE_BEGIN

class SFPlugin_Profiler : public SFPlugin
{
public:
    SFPlugin_Profiler(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_Profiler() {};

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
