/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SSFObject_ClientSocket.cpp
**作者: shyfan
**日期: 2023/09/06 20:16:33
**功能: 客户端套接字对象
*************************************************************************/

#include "SSFObject_ClientSocket.h"

#include "Include/SFILog.h"

#include <vector>
#include <chrono>
#include <cerrno>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFObject_ClientSocket, ESFLogLevel::Debug);

SSFObject_ClientSocket::SSFObject_ClientSocket(SSFNetworkSocketCreatorContext &InContext, SFObjectErrors &InErrors)
    : SSFObject_NetworkSocket(InContext, InErrors)
{
    LastRecvMS = GetSteadyNowMS();
}

SSFObject_ClientSocket::~SSFObject_ClientSocket()
{
}

void SSFObject_ClientSocket::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFObject_NetworkSocket::Tick(Errors, DelayMS);

    // 获取客户端套接字
    SSFSOCKET ClientSocket = GetSocket();

    // 检查客户端套接字是否有效
    if (IsSocketInvalid())
    {
        return;
    }

    // TODO Shyfan 检查客户端套接字连接状态

    // 与客户端进行数据通信
    char buffer[1024];
    int bytesReceived = SSF_SOCKET_READ(ClientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0)
    {
#if defined(SKYWALKER_PLATFORM_WINDOWS)
        if (bytesReceived == 0)
        {
            bClosed = true;
        }
        else
        {
            int Error = WSAGetLastError();
            if (Error != WSAEWOULDBLOCK)
            {
                bClosed = true;
            }
        }
#else
        if (bytesReceived == 0)
        {
            bClosed = true;
        }
        else if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            bClosed = true;
        }
#endif
        return;
    }

    LastRecvMS = GetSteadyNowMS();

    NetworkCodec.Append(reinterpret_cast<const SFByte *>(buffer), static_cast<SFUInt32>(bytesReceived));

    SSFNetworkPacket Packet;
    while (NetworkCodec.TryDecode(Packet))
    {
        SF_LOG_DEBUG("ClientSocket " << ClientSocket << " Recv MsgId " << Packet.MsgId << " Seq " << Packet.Seq << " BodyLen " << Packet.BodyLen)
        RecvPackets.push_back(Packet);
    }

    if (NetworkCodec.HasFatalError())
    {
        ++InvalidPacketCount;
        bClosed = true;
        NetworkCodec.ResetFatalError();
    }
}

bool SSFObject_ClientSocket::SendPacket(const SSFNetworkPacket &Packet)
{
    SSFSOCKET ClientSocket = GetSocket();
    if (ClientSocket == SSF_INVALID_SOCKET)
    {
        return false;
    }

    std::vector<SFByte> Buffer;
    if (!SSFNetworkCodec::Encode(Packet, Buffer))
    {
        return false;
    }

    int SentBytes = SSF_SOCKET_WRITE(ClientSocket, reinterpret_cast<const char *>(Buffer.data()), static_cast<int>(Buffer.size()), 0);
    if (SentBytes == static_cast<int>(Buffer.size()) && Packet.MsgId == static_cast<SFUInt16>(ESFNetworkMsg::S2C_HeartbeatAck))
    {
        LastHeartbeatMS = GetSteadyNowMS();
    }
    return SentBytes == static_cast<int>(Buffer.size());
}

void SSFObject_ClientSocket::FetchPackets(std::vector<SSFNetworkPacket> &OutPackets)
{
    OutPackets.insert(OutPackets.end(), RecvPackets.begin(), RecvPackets.end());
    RecvPackets.clear();
}

SFUInt64 SSFObject_ClientSocket::GetSteadyNowMS() const
{
    return static_cast<SFUInt64>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                     std::chrono::steady_clock::now().time_since_epoch())
                                     .count());
}
