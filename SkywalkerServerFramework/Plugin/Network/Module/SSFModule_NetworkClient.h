/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Network\Module\SSFModule_NetworkClient.h
**作者: shyfan
**日期: 2023/08/26 15:37:38
**功能: 网络客户端模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__

#include "Include/SSFCore.h"

#include "Core/Module/SSFModule.h"

SSF_NAMESPACE_BEGIN

class SSFModule_NetworkClient : public SSFModule
{
    SSF_OBJECT_CLASS(SSFModule_NetworkClient)

public:
    SSFModule_NetworkClient(SSFModuleContext &InContext, SSFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_NetworkClient(){};
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_NETWORK_CLIENT_H__
