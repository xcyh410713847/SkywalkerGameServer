/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\SSFPlugin_Network.h
**作者: shyfan
**日期: 2023/08/15 20:04:42
**功能: 网络插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Plugin/SSFPlugin.h"

#pragma comment(lib, "ws2_32.lib")

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFPlugin_Network : public SSFOPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_Network)

public:
    SSFPlugin_Network(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOPlugin(InPluginManager)
    {
    }
    virtual ~SSFPlugin_Network(){};

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFPlugin_Network);
    };

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

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__
