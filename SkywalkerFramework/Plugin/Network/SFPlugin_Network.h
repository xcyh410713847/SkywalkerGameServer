/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\SFPlugin_Network.h
**作者: shyfan
**日期: 2023/08/15 20:04:42
**功能: 网络插件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__

#include "Core/Plugin/SFPlugin.h"

#pragma comment(lib, "ws2_32.lib")

SF_NAMESPACE_BEGIN

class SFPlugin_Network : public SFPlugin
{
public:
    SFPlugin_Network(SFPluginContext &InContext, SFObjectErrors &InErrors)
        : SFPlugin(InContext, InErrors)
    {
    }
    virtual ~SFPlugin_Network() {};

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

#endif // __SKYWALKER_SERVER_FRAMEWORK_PLUGIN_NETWORK_H__
