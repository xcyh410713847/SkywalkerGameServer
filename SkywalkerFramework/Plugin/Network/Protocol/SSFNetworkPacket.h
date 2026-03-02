/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Protocol\SSFNetworkPacket.h
**作者: shyfan
**日期: 2026/03/02
**功能: 网络包定义
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_PACKET_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_PACKET_H__

#include "Include/SFCore.h"

#include <vector>

SF_NAMESPACE_BEGIN

/**
 * 网络消息类型
 */
enum class ESFNetworkMsg : SFUInt16
{
    Unknown = 0,
    C2S_Heartbeat = 1001,
    S2C_HeartbeatAck = 1002,
    C2S_Login = 1101,
    S2C_LoginAck = 1102,
    C2S_PlayerInput = 1201,
    S2C_PlayerState = 1202,
};

/**
 * 网络包头长度
 */
static constexpr SFUInt32 SF_NETWORK_PACKET_HEADER_SIZE = 16;

/**
 * 网络包魔数
 */
static constexpr SFUInt16 SF_NETWORK_PACKET_MAGIC = 0x5346;

/**
 * 网络协议版本
 */
static constexpr SFUInt16 SF_NETWORK_PACKET_VERSION = 1;

/**
 * 包体最大长度（64KB）
 */
static constexpr SFUInt32 SF_NETWORK_PACKET_MAX_BODY_SIZE = 64 * 1024;

/**
 * 网络包
 */
struct SSFNetworkPacket
{
    SFUInt16 Magic = SF_NETWORK_PACKET_MAGIC;
    SFUInt16 Version = SF_NETWORK_PACKET_VERSION;
    SFUInt16 MsgId = 0;
    SFUInt16 Flags = 0;
    SFUInt32 Seq = 0;
    SFUInt32 BodyLen = 0;

    std::vector<SFByte> Body;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_NETWORK_PACKET_H__
