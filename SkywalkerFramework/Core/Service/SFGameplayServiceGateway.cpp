/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFGameplayServiceGateway.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 游戏业务服务网关
*************************************************************************/

#include "SFGameplayServiceGateway.h"

SF_NAMESPACE_USING

/** 获取网关单例 */
SSFGameplayServiceGateway &SSFGameplayServiceGateway::Instance()
{
    static SSFGameplayServiceGateway Gateway;
    return Gateway;
}

void SSFGameplayServiceGateway::RegisterAuthValidator(const AuthValidator &InValidator)
{
    Validator = InValidator;
}

void SSFGameplayServiceGateway::RegisterPlayerLoader(const PlayerLoader &InLoader)
{
    Loader = InLoader;
}

void SSFGameplayServiceGateway::RegisterWorldEnter(const WorldEnter &InEnter)
{
    Enter = InEnter;
}

void SSFGameplayServiceGateway::RegisterWorldLeave(const WorldLeave &InLeave)
{
    Leave = InLeave;
}

bool SSFGameplayServiceGateway::ValidateToken(const SFString &Token) const
{
    // 未注册回调时返回 false，确保默认拒绝
    if (!Validator)
    {
        return false;
    }

    return Validator(Token);
}

bool SSFGameplayServiceGateway::LoadPlayer(SFUInt64 PlayerId) const
{
    if (!Loader)
    {
        return false;
    }

    return Loader(PlayerId);
}

bool SSFGameplayServiceGateway::EnterWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const
{
    if (!Enter)
    {
        return false;
    }

    return Enter(PlayerId, WorldId);
}

bool SSFGameplayServiceGateway::LeaveWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const
{
    if (!Leave)
    {
        return false;
    }

    return Leave(PlayerId, WorldId);
}
