/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\SSFPlugin_Network.h
**作者: shyfan
**日期: 2023/08/15 20:04:42
**功能: 网络插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__

#include "Core/Plugin/SSFPlugin.h"

#pragma comment(lib, "ws2_32.lib")

SSF_NAMESPACE_BEGIN

class SSFPlugin_Network : public SSFPlugin
{
    SSF_OBJECT_CLASS(SSFPlugin_Network)

public:
    SSFPlugin_Network(SSFPluginContext &InContext, SSFObjectErrors &InErrors)
        : SSFPlugin(InContext, InErrors)
    {
    }
    virtual ~SSFPlugin_Network(){};

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

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__
