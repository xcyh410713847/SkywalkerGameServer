/*************************************************************************
**文件: SkywalkerFramework\Include\SFNetworkInterface.h
**作者: shyfan
**日期: 2026/03/19
**功能: 网络服务公共接口（供跨插件访问）
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFNETWORK_INTERFACE_H__
#define __SKYWALKER_FRAMEWORK_SFNETWORK_INTERFACE_H__

#include "SFCore.h"

#include <functional>
#include <vector>

SF_NAMESPACE_BEGIN

/*------------------------------------------------------------------------
 * 消息 ID 类型（与 Network 插件内的 SFMessageCodec 一致）
 *----------------------------------------------------------------------*/
typedef SFUInt16 SFMsgID;

/*------------------------------------------------------------------------
 * 消息处理回调签名
 *----------------------------------------------------------------------*/
using SFMessageHandler = std::function<void(SFUInt32 SessionId,
                                            const char *Payload,
                                            SFUInt32 PayloadLen)>;

/*------------------------------------------------------------------------
 * 全局消息 ID 分配
 *----------------------------------------------------------------------*/

/* 系统消息 0x0001 - 0x00FF (Network 自用) */
static const SFMsgID SF_MSGID_HEARTBEAT     = 0x0001;

/* 登录/鉴权 0x0100 - 0x01FF (Auth) */
static const SFMsgID SF_MSGID_LOGIN_REQ     = 0x0101;
static const SFMsgID SF_MSGID_LOGIN_RESP    = 0x0102;

/* 场景/世界 0x0200 - 0x02FF (Level) */
static const SFMsgID SF_MSGID_ENTER_SCENE_REQ   = 0x0201;
static const SFMsgID SF_MSGID_ENTER_SCENE_RESP  = 0x0202;
static const SFMsgID SF_MSGID_ACTOR_ENTER_SCENE = 0x0203;
static const SFMsgID SF_MSGID_ACTOR_LEAVE_SCENE = 0x0204;

/* 角色/移动 0x0300 - 0x03FF (Level) */
static const SFMsgID SF_MSGID_MOVE_REQ         = 0x0301;
static const SFMsgID SF_MSGID_MOVE_BROADCAST   = 0x0302;

/* 游戏业务 0x0400 - 0x04FF (Game) */
static const SFMsgID SF_MSGID_PLAYER_ENTER     = 0x0401;
static const SFMsgID SF_MSGID_PLAYER_LEAVE     = 0x0402;

/* AI/生物 0x0500 - 0x05FF (AI) */
static const SFMsgID SF_MSGID_CREATURE_SPAWN   = 0x0501;

/*------------------------------------------------------------------------
 * LoginReq / LoginResp 负载结构
 *
 * LoginReq:  PlayerId(u32) + TokenLen(u16) + Token(bytes)
 * LoginResp: Result(u8) + PlayerSessionId(u32)
 *   Result: 0=成功, 1=Token错误, 2=已在线, 3=服务器满
 *----------------------------------------------------------------------*/

enum class ESFLoginResult : SFUInt8
{
    Success             = 0,
    InvalidToken        = 1,
    AlreadyOnline       = 2,
    ServerFull          = 3,
};

/*------------------------------------------------------------------------
 * ISFNetworkServer — 网络服务抽象接口
 *
 * 由 SFModule_NetworkServer 实现，其他插件通过 PluginManager 获取后
 * dynamic_cast 到此接口使用，避免直接包含 Network 插件头文件。
 *----------------------------------------------------------------------*/
class ISFNetworkServer
{
public:
    virtual ~ISFNetworkServer() = default;

    /** 注册消息处理器 */
    virtual bool RegisterHandler(SFMsgID MsgID, SFMessageHandler Handler) = 0;

    /** 注销消息处理器 */
    virtual void UnregisterHandler(SFMsgID MsgID) = 0;

    /** 发送消息给指定 Session */
    virtual bool SendTo(SFUInt32 SessionId, SFMsgID MsgID,
                        const char *Payload, SFUInt32 PayloadLen) = 0;

    /** 广播消息给所有已认证 Session */
    virtual void Broadcast(SFMsgID MsgID,
                           const char *Payload, SFUInt32 PayloadLen) = 0;

    /** 广播消息给指定 Session 列表 */
    virtual void BroadcastTo(const std::vector<SFUInt32> &SessionIds,
                             SFMsgID MsgID,
                             const char *Payload, SFUInt32 PayloadLen) = 0;

    /** 设置 Session 为已认证状态 */
    virtual void SetSessionAuthenticated(SFUInt32 SessionId, SFUInt32 PlayerId) = 0;

    /** 获取 Session 是否已认证 */
    virtual bool IsSessionAuthenticated(SFUInt32 SessionId) = 0;

    /** 获取 Session 的 PlayerId */
    virtual SFUInt32 GetSessionPlayerId(SFUInt32 SessionId) = 0;

    /** 根据 PlayerId 查找 SessionId (0=未找到) */
    virtual SFUInt32 FindSessionByPlayerId(SFUInt32 PlayerId) = 0;

    /** 关闭指定 Session */
    virtual void CloseSession(SFUInt32 SessionId) = 0;

    /** 本地分发消息（不经过网络接收，仅触发服务器侧处理器） */
    virtual bool DispatchLocal(SFUInt32 SessionId, SFMsgID MsgID,
                               const char *Payload, SFUInt32 PayloadLen) = 0;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_FRAMEWORK_SFNETWORK_INTERFACE_H__ */
