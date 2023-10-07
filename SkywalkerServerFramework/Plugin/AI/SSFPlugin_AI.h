/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\AI\SSFPlugin_AI.h
**作者: shyfan
**日期: 2023/10/07 20:20:37
**功能: AI插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Plugin/SSFPlugin.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFPlugin_AI : public SSFOPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_AI)

public:
    SSFPlugin_AI(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOPlugin(InPluginManager)
    {
    }
    virtual ~SSFPlugin_AI(){};

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFPlugin_AI);
    }

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

SKYWALKER_SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_AI_H__
