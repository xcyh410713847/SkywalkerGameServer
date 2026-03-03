/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Router\SFNetworkRouter.h
**作者: shyfan
**日期: 2026/03/02
**功能: 网络消息路由
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_ROUTER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_ROUTER_H__

#include "../Protocol/SFNetworkPacket.h"

#include <functional>

SF_NAMESPACE_BEGIN

class SSFNetworkRouter
{
public:
    typedef std::function<void(const SSFNetworkPacket &Packet)> RouterHandler;

    /**
     * 注册消息处理器
     */
    void RegisterHandler(SFUInt16 MsgId, const RouterHandler &Handler);

    /**
     * 路由消息
     */
    bool Route(const SSFNetworkPacket &Packet) const;

private:
    SFMap<SFUInt16, RouterHandler> HandlerMap;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_NETWORK_ROUTER_H__
