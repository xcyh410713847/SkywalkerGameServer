/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Video\SSFPlugin_Video.h
**作者: shyfan
**日期: 2023/09/25 23:01:57
**功能: 录像插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Plugin/SSFPlugin.h"

SSF_NAMESPACE_BEGIN

class SSFPlugin_Video : public SSFOPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_Video)

public:
    SSFPlugin_Video(SSF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOPlugin(InPluginManager)
    {
    }
    virtual ~SSFPlugin_Video(){};

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName() override
    {
        return SSF_CLASS_NAME(SSFPlugin_Video);
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

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFPlugin_Video_H__
