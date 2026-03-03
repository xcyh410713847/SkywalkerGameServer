/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Protocol\SFNetworkLoginPayload.h
**作者: shyfan
**日期: 2026/03/02
**功能: 登录消息载荷
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_LOGIN_PAYLOAD_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_LOGIN_PAYLOAD_H__

#include "Include/SFCore.h"

#include <vector>

SF_NAMESPACE_BEGIN

struct SSFNetworkLoginPayload
{
    SFUInt64 PlayerId = 0;
    SFUInt64 WorldId = 0;
    SFString Token;
};

class SSFNetworkLoginPayloadCodec
{
public:
    static bool Encode(const SSFNetworkLoginPayload &Payload, std::vector<SFByte> &OutBody);
    static bool Decode(const std::vector<SFByte> &Body, SSFNetworkLoginPayload &OutPayload);
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_NETWORK_LOGIN_PAYLOAD_H__
