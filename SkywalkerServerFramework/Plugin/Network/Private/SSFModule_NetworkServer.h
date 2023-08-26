/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Private\SSFModule_NetworkServer.h
**作者: shyfan
**日期: 2023/08/26 09:18:17
**功能: 网络服务器模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOModule_NetworkServer : public SSFOModule
{
public:
    SSFOModule_NetworkServer(SKYWALKER_SF_PTR_PLUGIN_MANAGER InPluginManager)
        : SSFOModule(InPluginManager)
    {
    }
    virtual ~SSFOModule_NetworkServer(){};

    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName() override
    {
        return SKYWALKER_SF_CLASS_NAME(SSFOModule_NetworkServer);
    };
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_SERVER_H__
