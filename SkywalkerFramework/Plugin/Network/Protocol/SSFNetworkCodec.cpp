/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Protocol\SSFNetworkCodec.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 网络包编解码
*************************************************************************/

#include "SSFNetworkCodec.h"

#include <cstring>
#include <limits>

SF_NAMESPACE_USING

namespace
{
    void WriteUInt16(std::vector<SFByte> &Buffer, SFUInt16 Value)
    {
        SFByte Bytes[2] = {};
        std::memcpy(Bytes, &Value, sizeof(Value));
        Buffer.insert(Buffer.end(), Bytes, Bytes + 2);
    }

    void WriteUInt32(std::vector<SFByte> &Buffer, SFUInt32 Value)
    {
        SFByte Bytes[4] = {};
        std::memcpy(Bytes, &Value, sizeof(Value));
        Buffer.insert(Buffer.end(), Bytes, Bytes + 4);
    }

    SFUInt16 ReadUInt16(const SFByte *Data)
    {
        SFUInt16 Value = 0;
        std::memcpy(&Value, Data, sizeof(Value));
        return Value;
    }

    SFUInt32 ReadUInt32(const SFByte *Data)
    {
        SFUInt32 Value = 0;
        std::memcpy(&Value, Data, sizeof(Value));
        return Value;
    }
}

void SSFNetworkCodec::Append(SF_CONST_PTR(SFByte) Data, SFUInt32 Length)
{
    if (Data == nullptr || Length == 0)
    {
        return;
    }

    RecvBuffer.insert(RecvBuffer.end(), Data, Data + Length);
}

bool SSFNetworkCodec::TryDecode(SSFNetworkPacket &Packet)
{
    LastDecodeError = ESFNetworkDecodeError::None;

    if (RecvBuffer.size() < SF_NETWORK_PACKET_HEADER_SIZE)
    {
        LastDecodeError = ESFNetworkDecodeError::NeedMoreData;
        return false;
    }

    const SFByte *HeaderPtr = RecvBuffer.data();

    Packet.Magic = ReadUInt16(HeaderPtr + 0);
    Packet.Version = ReadUInt16(HeaderPtr + 2);
    Packet.MsgId = ReadUInt16(HeaderPtr + 4);
    Packet.Flags = ReadUInt16(HeaderPtr + 6);
    Packet.Seq = ReadUInt32(HeaderPtr + 8);
    Packet.BodyLen = ReadUInt32(HeaderPtr + 12);

    if (Packet.Magic != SF_NETWORK_PACKET_MAGIC)
    {
        LastDecodeError = ESFNetworkDecodeError::InvalidMagic;
        RecvBuffer.erase(RecvBuffer.begin());
        return false;
    }

    if (Packet.Version != SF_NETWORK_PACKET_VERSION)
    {
        LastDecodeError = ESFNetworkDecodeError::InvalidVersion;
        RecvBuffer.erase(RecvBuffer.begin());
        return false;
    }

    if (Packet.BodyLen > SF_NETWORK_PACKET_MAX_BODY_SIZE)
    {
        LastDecodeError = ESFNetworkDecodeError::BodyTooLarge;
        bFatalError = true;
        RecvBuffer.clear();
        return false;
    }

    const SFUInt32 TotalPacketLen = SF_NETWORK_PACKET_HEADER_SIZE + Packet.BodyLen;
    if (RecvBuffer.size() < TotalPacketLen)
    {
        LastDecodeError = ESFNetworkDecodeError::NeedMoreData;
        return false;
    }

    Packet.Body.clear();
    if (Packet.BodyLen > 0)
    {
        Packet.Body.insert(Packet.Body.end(),
                           RecvBuffer.begin() + SF_NETWORK_PACKET_HEADER_SIZE,
                           RecvBuffer.begin() + TotalPacketLen);
    }

    RecvBuffer.erase(RecvBuffer.begin(), RecvBuffer.begin() + TotalPacketLen);
    return true;
}

bool SSFNetworkCodec::Encode(const SSFNetworkPacket &Packet, std::vector<SFByte> &OutBuffer)
{
    if (Packet.Body.size() > SF_NETWORK_PACKET_MAX_BODY_SIZE)
    {
        return false;
    }

    if (Packet.Body.size() > static_cast<size_t>((std::numeric_limits<SFUInt32>::max)()))
    {
        return false;
    }

    OutBuffer.clear();
    OutBuffer.reserve(SF_NETWORK_PACKET_HEADER_SIZE + Packet.Body.size());

    WriteUInt16(OutBuffer, Packet.Magic);
    WriteUInt16(OutBuffer, Packet.Version);
    WriteUInt16(OutBuffer, Packet.MsgId);
    WriteUInt16(OutBuffer, Packet.Flags);
    WriteUInt32(OutBuffer, Packet.Seq);
    WriteUInt32(OutBuffer, static_cast<SFUInt32>(Packet.Body.size()));

    if (!Packet.Body.empty())
    {
        OutBuffer.insert(OutBuffer.end(), Packet.Body.begin(), Packet.Body.end());
    }

    return true;
}
