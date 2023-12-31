﻿/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Profiler\SSFPlugin_Profiler.h
**作者: shyfan
**日期: 2023/09/25 20:20:26
**功能: 分析器插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__

#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_Profiler : public SSFOPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_Profiler)

public:
    SSFPlugin_Profiler(SSF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOPlugin(InPluginManager)
    {
    }
    virtual ~SSFPlugin_Profiler(){};

#pragma region SSFOPlugin

private:
    /**
     * 安装
     */
    virtual void Install(SSFModuleErrors &Errors) override;

    /**
     * 卸载
     */
    virtual void Uninstall(SSFModuleErrors &Errors) override;

#pragma endregion SSFOPlugin
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_PROFILER_H__
