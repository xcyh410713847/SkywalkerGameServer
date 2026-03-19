/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFMessageCodec.h
**作者: shyfan
**日期: 2026/03/19
**功能: 消息帧编解码器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MESSAGE_CODEC_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MESSAGE_CODEC_H__

#include "Include/SFCore.h"

#include "SFNetworkBuffer.h"

#include <vector>

SF_NAMESPACE_BEGIN

/**
 * 消息帧格式:
 *   [Length:4bytes uint32 网络序] [MsgID:2bytes uint16 网络序] [Payload:N bytes]
 *   Length = sizeof(MsgID) + sizeof(Payload) = 2 + N
 *   总包长 = 4 + Length
 */

/** 长度字段大小 */
static const SFSize SF_MSG_LENGTH_SIZE = 4;
/** 消息ID字段大小 */
static const SFSize SF_MSG_ID_SIZE = 2;
/** 消息头大小 = Length + MsgID */
static const SFSize SF_MSG_HEADER_SIZE = SF_MSG_LENGTH_SIZE + SF_MSG_ID_SIZE;
/** 单包最大 Payload 长度（防御性限制） */
static const SFUInt32 SF_MSG_MAX_PAYLOAD = 65536;

/** 消息 ID 类型 */
typedef SFUInt16 SFMsgID;

/** 解码出的单条消息 */
struct SFDecodedMessage
{
    SFMsgID MsgID;
    const char *Payload;
    SFUInt32 PayloadLen;
};

/**
 * 消息编解码器（无状态工具类）
 *
 * Encode: 将 MsgID + Payload 编码为带长度前缀的帧并写入 SendBuffer
 * Decode: 从 RecvBuffer 中尝试解出完整消息帧
 */
class SFMessageCodec
{
public:
    /**
     * 编码一条消息到发送缓冲区
     * @param SendBuffer 发送缓冲区
     * @param MsgID      消息ID
     * @param Payload    负载数据（可为 nullptr 表示无负载）
     * @param PayloadLen 负载长度
     * @return true=成功, false=负载过大
     */
    static bool Encode(SFNetworkBuffer &SendBuffer, SFMsgID MsgID,
                       const char *Payload, SFUInt32 PayloadLen)
    {
        if (PayloadLen > SF_MSG_MAX_PAYLOAD)
        {
            return false;
        }

        /* Length 字段 = MsgID(2) + PayloadLen */
        SFUInt32 Length = static_cast<SFUInt32>(SF_MSG_ID_SIZE + PayloadLen);
        SFUInt32 LengthNO = HostToNetwork32(Length);
        SFUInt16 MsgIDNO = HostToNetwork16(MsgID);

        SendBuffer.Append(reinterpret_cast<const char *>(&LengthNO), SF_MSG_LENGTH_SIZE);
        SendBuffer.Append(reinterpret_cast<const char *>(&MsgIDNO), SF_MSG_ID_SIZE);
        if (Payload != nullptr && PayloadLen > 0)
        {
            SendBuffer.Append(Payload, PayloadLen);
        }
        return true;
    }

    /**
     * 从接收缓冲区解码消息
     * @param RecvBuffer 接收缓冲区
     * @param OutMessages 解出的消息列表（追加到尾部）
     * @return 解出的消息数量；缓冲区数据不足时返回 0
     *
     * 注意: OutMessages 中的 Payload 指针指向 RecvBuffer 内部，
     *       在下次 Consume 后失效。调用方应在循环结束后统一 Consume。
     */
    static SFUInt32 Decode(SFNetworkBuffer &RecvBuffer,
                           std::vector<SFDecodedMessage> &OutMessages)
    {
        SFUInt32 Count = 0;
        SFSize Consumed = 0;

        while (true)
        {
            SFSize Readable = RecvBuffer.ReadableBytes() - Consumed;
            const char *Ptr = RecvBuffer.ReadPtr() + Consumed;

            /* 不够读 Length 字段 */
            if (Readable < SF_MSG_LENGTH_SIZE)
            {
                break;
            }

            SFUInt32 LengthNO = 0;
            std::memcpy(&LengthNO, Ptr, SF_MSG_LENGTH_SIZE);
            SFUInt32 Length = NetworkToHost32(LengthNO);

            /* 防御：Length 必须 >= MsgID(2)，且 Payload 不超限 */
            if (Length < SF_MSG_ID_SIZE || (Length - SF_MSG_ID_SIZE) > SF_MSG_MAX_PAYLOAD)
            {
                /* 协议错误，消费掉整个缓冲区以断开 */
                Consumed = RecvBuffer.ReadableBytes();
                break;
            }

            SFSize FrameSize = SF_MSG_LENGTH_SIZE + Length;
            if (Readable < FrameSize)
            {
                /* 数据不完整，等下次 recv */
                break;
            }

            /* 解码 MsgID */
            SFUInt16 MsgIDNO = 0;
            std::memcpy(&MsgIDNO, Ptr + SF_MSG_LENGTH_SIZE, SF_MSG_ID_SIZE);

            SFDecodedMessage Msg;
            Msg.MsgID = NetworkToHost16(MsgIDNO);
            Msg.PayloadLen = Length - static_cast<SFUInt32>(SF_MSG_ID_SIZE);
            Msg.Payload = (Msg.PayloadLen > 0) ? (Ptr + SF_MSG_HEADER_SIZE) : nullptr;

            OutMessages.push_back(Msg);
            Consumed += FrameSize;
            ++Count;
        }

        if (Consumed > 0)
        {
            RecvBuffer.Consume(Consumed);
        }

        return Count;
    }

private:
    /** 主机序 → 网络序 (big-endian) 32 位 */
    static SFUInt32 HostToNetwork32(SFUInt32 Host)
    {
        SFUInt32 Net = 0;
        unsigned char *P = reinterpret_cast<unsigned char *>(&Net);
        P[0] = static_cast<unsigned char>((Host >> 24) & 0xFF);
        P[1] = static_cast<unsigned char>((Host >> 16) & 0xFF);
        P[2] = static_cast<unsigned char>((Host >> 8) & 0xFF);
        P[3] = static_cast<unsigned char>(Host & 0xFF);
        return Net;
    }

    /** 网络序 → 主机序 32 位 */
    static SFUInt32 NetworkToHost32(SFUInt32 Net)
    {
        const unsigned char *P = reinterpret_cast<const unsigned char *>(&Net);
        return (static_cast<SFUInt32>(P[0]) << 24) |
               (static_cast<SFUInt32>(P[1]) << 16) |
               (static_cast<SFUInt32>(P[2]) << 8) |
               static_cast<SFUInt32>(P[3]);
    }

    /** 主机序 → 网络序 16 位 */
    static SFUInt16 HostToNetwork16(SFUInt16 Host)
    {
        SFUInt16 Net = 0;
        unsigned char *P = reinterpret_cast<unsigned char *>(&Net);
        P[0] = static_cast<unsigned char>((Host >> 8) & 0xFF);
        P[1] = static_cast<unsigned char>(Host & 0xFF);
        return Net;
    }

    /** 网络序 → 主机序 16 位 */
    static SFUInt16 NetworkToHost16(SFUInt16 Net)
    {
        const unsigned char *P = reinterpret_cast<const unsigned char *>(&Net);
        return (static_cast<SFUInt16>(P[0]) << 8) |
               static_cast<SFUInt16>(P[1]);
    }
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MESSAGE_CODEC_H__ */
