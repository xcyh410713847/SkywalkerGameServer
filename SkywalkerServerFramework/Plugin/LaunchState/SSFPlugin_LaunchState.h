/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\LaunchState\SSFPlugin_LaunchState.h
**作者: shyfan
**日期: 2023/07/30 12:38:38
**功能: 启动状态插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_LAUNCH_STATE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_LAUNCH_STATE_H__

#include "Include/SSFCore.h"

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Plugin/SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOPlugin_LaunchState : public SSFOPlugin
{
#pragma region Object
#pragma endregion Object

public:
    SSFOPlugin_LaunchState(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOPlugin(InPluginManager)
    {
    }
    virtual ~SSFOPlugin_LaunchState(){};

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFOPlugin_LaunchState);
    }
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_LAUNCH_STATE_H__
