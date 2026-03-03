/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFObject_ClientSocket.h
**作者: shyfan
**日期: 2023/09/06 20:16:16
**功能: 客户端套接字对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_CLIENT_SOCKET_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_CLIENT_SOCKET_H__

#include "Include/SFCore.h"

#include "SFObject_NetworkSocket.h"

#include "../Protocol/SFNetworkCodec.h"

#include <vector>

SF_NAMESPACE_BEGIN

class SSFObject_ClientSocket : public SSFObject_NetworkSocket
{
public:
    SSFObject_ClientSocket(SSFNetworkSocketCreatorContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFObject_ClientSocket();

    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS) override;

    SFUInt64 GetLastRecvMS() const
    {
        return LastRecvMS;
    }

    SFUInt64 GetLastHeartbeatMS() const
    {
        return LastHeartbeatMS;
    }

    bool IsClosed() const
    {
        return bClosed;
    }

    SFUInt64 ConsumeInvalidPacketCount()
    {
        SFUInt64 Count = InvalidPacketCount;
        InvalidPacketCount = 0;
        return Count;
    }

    bool SendPacket(const SSFNetworkPacket &Packet);

    void FetchPackets(std::vector<SSFNetworkPacket> &OutPackets);

private:
    SFUInt64 GetSteadyNowMS() const;

private:
    SSFNetworkCodec NetworkCodec;
    std::vector<SSFNetworkPacket> RecvPackets;

    SFUInt64 LastRecvMS = 0;
    SFUInt64 LastHeartbeatMS = 0;
    bool bClosed = false;
    SFUInt64 InvalidPacketCount = 0;
};

SF_NAMESPACE_END

#define SSF_PRT_CLIENT_SOCKET SF_PTR(SSFObject_ClientSocket)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_CLIENT_SOCKET_H__
