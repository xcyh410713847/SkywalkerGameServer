/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Protocol\SFNetworkLoginPayload.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 登录消息载荷
*************************************************************************/

#include "SFNetworkLoginPayload.h"

#include <sstream>

SF_NAMESPACE_USING

bool SSFNetworkLoginPayloadCodec::Encode(const SSFNetworkLoginPayload &Payload, std::vector<SFByte> &OutBody)
{
    if (Payload.PlayerId == 0 || Payload.WorldId == 0 || Payload.Token.empty())
    {
        return false;
    }

    std::ostringstream Stream;
    Stream << Payload.PlayerId << "|" << Payload.WorldId << "|" << Payload.Token;
    SFString Data = Stream.str();

    OutBody.assign(Data.begin(), Data.end());
    return true;
}

bool SSFNetworkLoginPayloadCodec::Decode(const std::vector<SFByte> &Body, SSFNetworkLoginPayload &OutPayload)
{
    if (Body.empty())
    {
        return false;
    }

    SFString Data(Body.begin(), Body.end());
    std::istringstream Stream(Data);

    SFString PlayerIdStr;
    SFString WorldIdStr;
    SFString Token;

    if (!std::getline(Stream, PlayerIdStr, '|'))
    {
        return false;
    }

    if (!std::getline(Stream, WorldIdStr, '|'))
    {
        return false;
    }

    if (!std::getline(Stream, Token))
    {
        return false;
    }

    if (PlayerIdStr.empty() || WorldIdStr.empty() || Token.empty())
    {
        return false;
    }

    OutPayload.PlayerId = std::stoull(PlayerIdStr);
    OutPayload.WorldId = std::stoull(WorldIdStr);
    OutPayload.Token = Token;
    return (OutPayload.PlayerId != 0 && OutPayload.WorldId != 0);
}
