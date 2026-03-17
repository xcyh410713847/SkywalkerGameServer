/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFGameplayServiceGateway.h
**作者: shyfan
**日期: 2026/03/02
**功能: 游戏业务服务网关
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__

#include "Include/SFCore.h"

#include <functional>

SF_NAMESPACE_BEGIN

/**
 * 游戏业务服务网关（单例）
 *
 * 说明：
 * 1. 通过 std::function 注册业务回调
 * 2. 向上层提供统一调用入口
 * 3. 当回调未注册时返回安全默认值
 */
class SSFGameplayServiceGateway
{
public:
    using AuthValidator = std::function<bool(const SFString &Token)>;
    using PlayerLoader = std::function<bool(SFUInt64 PlayerId)>;
    using WorldEnter = std::function<bool(SFUInt64 PlayerId, SFUInt64 WorldId)>;
    using WorldLeave = std::function<bool(SFUInt64 PlayerId, SFUInt64 WorldId)>;

public:
    /** 获取单例实例 */
    static SSFGameplayServiceGateway &Instance();

    /** 注册鉴权回调 */
    void RegisterAuthValidator(const AuthValidator &InValidator);
    /** 注册加载玩家回调 */
    void RegisterPlayerLoader(const PlayerLoader &InLoader);
    /** 注册进入世界回调 */
    void RegisterWorldEnter(const WorldEnter &InEnter);
    /** 注册离开世界回调 */
    void RegisterWorldLeave(const WorldLeave &InLeave);

    bool ValidateToken(const SFString &Token) const;
    bool LoadPlayer(SFUInt64 PlayerId) const;
    bool EnterWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const;
    bool LeaveWorld(SFUInt64 PlayerId, SFUInt64 WorldId) const;

private:
    AuthValidator Validator;
    PlayerLoader Loader;
    WorldEnter Enter;
    WorldLeave Leave;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFGAMEPLAYSERVICEGATEWAY_H__
