/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Router\SFNetworkRouter.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 网络消息路由
*************************************************************************/

#include "SFNetworkRouter.h"

SF_NAMESPACE_USING

void SSFNetworkRouter::RegisterHandler(SFUInt16 MsgId, const RouterHandler &Handler)
{
    if (!Handler)
    {
        return;
    }

    HandlerMap[MsgId] = Handler;
}

bool SSFNetworkRouter::Route(const SSFNetworkPacket &Packet) const
{
    auto Iter = HandlerMap.find(Packet.MsgId);
    if (Iter == HandlerMap.end())
    {
        return false;
    }

    Iter->second(Packet);
    return true;
}
