/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\Module\SFModule_Auth.cpp
**作者: shyfan
**日期: 2026/03/19
**功能: 登录鉴权模块
*************************************************************************/

#include "SFModule_Auth.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Plugin/SFPlugin.h"
#include "Core/Plugin/SFPluginManager.h"

#include <cstring>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_Auth, Framework);

#pragma region Object

void SFModule_Auth::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SFModule_Auth::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);

    /* 在 Awake 阶段查找网络服务并注册 Handler
     * 此时所有插件已 Init 完成，Network 的 Dispatcher 已创建 */
    NetworkServer = FindNetworkServer();
    if (NetworkServer == nullptr)
    {
        SF_LOG_ERROR("Auth module cannot find NetworkServer interface");
        return;
    }

    /* 注册 LoginReq 处理器 */
    NetworkServer->RegisterHandler(SF_MSGID_LOGIN_REQ,
        [this](SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
        {
            HandleLoginReq(SessionId, Payload, PayloadLen);
        });

    SF_LOG_FRAMEWORK("Auth module registered LoginReq handler");
}

void SFModule_Auth::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);
}

void SFModule_Auth::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
}

void SFModule_Auth::Stop(SFObjectErrors &Errors)
{
    if (NetworkServer != nullptr)
    {
        NetworkServer->UnregisterHandler(SF_MSGID_LOGIN_REQ);
    }

    SSFModule::Stop(Errors);
}

void SFModule_Auth::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_Auth::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

ISFNetworkServer *SFModule_Auth::FindNetworkServer()
{
    SF_PTR(SFPlugin) OwnerPlugin = GetPlugin();
    if (SF_PTR_INVALID(OwnerPlugin))
    {
        return nullptr;
    }

    SF_PTR(SFPluginManager) PluginMgr = OwnerPlugin->GetPluginManager();
    if (SF_PTR_INVALID(PluginMgr))
    {
        return nullptr;
    }

    /* 通过插件名找到 Network 插件 */
    SFString NetworkPluginName = "Skywalker::Framework::SFPlugin_Network";
    SF_PTR(SFPlugin) NetworkPlugin = PluginMgr->GetPlugin(NetworkPluginName);
    if (SF_PTR_INVALID(NetworkPlugin))
    {
        SF_LOG_ERROR("Cannot find plugin: " << NetworkPluginName);
        return nullptr;
    }

    /* 通过模块名找到 NetworkServer 模块 */
    SFString NetworkServerModuleName = "Skywalker::Framework::SFModule_NetworkServer";
    SF_PTR(SSFModule) NetworkServerModule = NetworkPlugin->GetModule(NetworkServerModuleName);
    if (SF_PTR_INVALID(NetworkServerModule))
    {
        SF_LOG_ERROR("Cannot find module: " << NetworkServerModuleName);
        return nullptr;
    }

    /* dynamic_cast 到 ISFNetworkServer 接口 */
    ISFNetworkServer *Interface = dynamic_cast<ISFNetworkServer *>(NetworkServerModule);
    if (Interface == nullptr)
    {
        SF_LOG_ERROR("NetworkServer module does not implement ISFNetworkServer");
    }

    return Interface;
}

void SFModule_Auth::HandleLoginReq(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
{
    if (NetworkServer == nullptr)
    {
        return;
    }

    /*
     * LoginReq payload:
     *   PlayerId (u32, network order)
     *   TokenLen (u16, network order)
     *   Token    (bytes)
     */
    if (PayloadLen < 6)
    {
        SF_LOG_ERROR("LoginReq payload too short from Session " << SessionId
                     << " len=" << PayloadLen);
        /* 回复失败 */
        char Resp[5] = {};
        Resp[0] = static_cast<char>(ESFLoginResult::InvalidToken);
        SFUInt32 NetSessionId = htonl(SessionId);
        std::memcpy(Resp + 1, &NetSessionId, 4);
        NetworkServer->SendTo(SessionId, SF_MSGID_LOGIN_RESP, Resp, 5);
        NetworkServer->CloseSession(SessionId);
        return;
    }

    /* 已认证的 Session 拒绝重复登录 */
    if (NetworkServer->IsSessionAuthenticated(SessionId))
    {
        SF_LOG_FRAMEWORK("Session " << SessionId << " already authenticated, ignore LoginReq");
        return;
    }

    /* 解析 PlayerId */
    SFUInt32 NetPlayerId = 0;
    std::memcpy(&NetPlayerId, Payload, 4);
    SFUInt32 PlayerId = ntohl(NetPlayerId);

    /* 解析 TokenLen + Token */
    SFUInt16 NetTokenLen = 0;
    std::memcpy(&NetTokenLen, Payload + 4, 2);
    SFUInt16 TokenLen = ntohs(NetTokenLen);

    if (static_cast<SFUInt32>(6 + TokenLen) > PayloadLen)
    {
        SF_LOG_ERROR("LoginReq token length mismatch from Session " << SessionId);
        char Resp[5] = {};
        Resp[0] = static_cast<char>(ESFLoginResult::InvalidToken);
        SFUInt32 NetSessId = htonl(SessionId);
        std::memcpy(Resp + 1, &NetSessId, 4);
        NetworkServer->SendTo(SessionId, SF_MSGID_LOGIN_RESP, Resp, 5);
        NetworkServer->CloseSession(SessionId);
        return;
    }

    SFString Token(Payload + 6, TokenLen);

    SF_LOG_FRAMEWORK("LoginReq from Session " << SessionId
                     << " PlayerId=" << PlayerId
                     << " Token=" << Token);

    /* 检查是否已有同 PlayerId 在线 */
    SFUInt32 ExistingSessionId = NetworkServer->FindSessionByPlayerId(PlayerId);
    if (ExistingSessionId != 0)
    {
        SF_LOG_FRAMEWORK("PlayerId " << PlayerId << " already online at Session " << ExistingSessionId
                         << ", kick old session");
        /* 踢掉旧连接 */
        NetworkServer->CloseSession(ExistingSessionId);
    }

    /* 验证 Token */
    if (!ValidateToken(PlayerId, Token))
    {
        SF_LOG_FRAMEWORK("Login failed for PlayerId " << PlayerId << " invalid token");
        char Resp[5] = {};
        Resp[0] = static_cast<char>(ESFLoginResult::InvalidToken);
        SFUInt32 NetSessId = htonl(SessionId);
        std::memcpy(Resp + 1, &NetSessId, 4);
        NetworkServer->SendTo(SessionId, SF_MSGID_LOGIN_RESP, Resp, 5);
        NetworkServer->CloseSession(SessionId);
        return;
    }

    /* 登录成功 */
    NetworkServer->SetSessionAuthenticated(SessionId, PlayerId);

    char Resp[5] = {};
    Resp[0] = static_cast<char>(ESFLoginResult::Success);
    SFUInt32 NetSessId = htonl(SessionId);
    std::memcpy(Resp + 1, &NetSessId, 4);
    NetworkServer->SendTo(SessionId, SF_MSGID_LOGIN_RESP, Resp, 5);

    char NotifyPayload[4] = {};
    SFUInt32 NotifyNetPlayerId = htonl(PlayerId);
    std::memcpy(NotifyPayload, &NotifyNetPlayerId, 4);
    NetworkServer->DispatchLocal(SessionId, SF_MSGID_PLAYER_ENTER, NotifyPayload, 4);

    SF_LOG_FRAMEWORK("Login success PlayerId=" << PlayerId << " SessionId=" << SessionId);
}

bool SFModule_Auth::ValidateToken(SFUInt32 PlayerId, const SFString &Token)
{
    /*
     * Demo 白名单验证：
     * Token 固定为 "skywalker_demo_token"
     * 所有 PlayerId > 0 且 Token 正确即通过
     */
    if (PlayerId == 0)
    {
        return false;
    }

    return Token == "skywalker_demo_token";
}
