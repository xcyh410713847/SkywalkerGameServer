/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\Module\SFModule_SceneManager.cpp
**作者: shyfan
**日期: 2026/03/19
**功能: 场景管理模块
*************************************************************************/

#include "SFModule_SceneManager.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFActorTypes.h"
#include "Include/SFNetworkInterface.h"

#include "Core/Plugin/SFPlugin.h"
#include "Core/Plugin/SFPluginManager.h"

#include <cstring>

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_SceneManager, Framework);

#pragma region Object

void SFModule_SceneManager::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SFModule_SceneManager::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);

    NetworkServer = FindNetworkServer();
    ActorFactory = FindActorFactory();
    if (NetworkServer == nullptr || ActorFactory == nullptr)
    {
        SF_LOG_ERROR("SceneManager dependencies missing");
        return;
    }

    NetworkServer->RegisterHandler(SF_MSGID_ENTER_SCENE_REQ,
        [this](SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
        {
            HandleEnterSceneReq(SessionId, Payload, PayloadLen);
        });

    NetworkServer->RegisterHandler(SF_MSGID_MOVE_REQ,
        [this](SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
        {
            HandleMoveReq(SessionId, Payload, PayloadLen);
        });

}

void SFModule_SceneManager::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    if (FindScene(DefaultSceneId) == nullptr)
    {
        SFScene *DefaultScene = new SFScene();
        DefaultScene->SceneId = DefaultSceneId;
        DefaultScene->SceneName = "DefaultScene";
        SceneMap.insert(std::make_pair(DefaultScene->SceneId, DefaultScene));

        SF_LOG_FRAMEWORK("Scene created: " << DefaultScene->SceneName << " SceneId=" << DefaultScene->SceneId);
    }
}

void SFModule_SceneManager::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
    if (ActorFactory != nullptr)
    {
        ActorFactory->TickAllActors(DelayMS);
    }
}

void SFModule_SceneManager::Stop(SFObjectErrors &Errors)
{
    if (NetworkServer != nullptr)
    {
        NetworkServer->UnregisterHandler(SF_MSGID_ENTER_SCENE_REQ);
        NetworkServer->UnregisterHandler(SF_MSGID_MOVE_REQ);
    }

    CleanupScenes();
    ActorSceneMap.clear();

    SSFModule::Stop(Errors);
}

void SFModule_SceneManager::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_SceneManager::Destroy(SFObjectErrors &Errors)
{
    CleanupScenes();
    ActorSceneMap.clear();
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

#pragma region ISFSceneManager

bool SFModule_SceneManager::AddActorToScene(SFUInt32 SceneId, SFActorId ActorId)
{
    SFScene *Scene = FindScene(SceneId);
    if (Scene == nullptr)
    {
        Scene = new SFScene();
        Scene->SceneId = SceneId;
        Scene->SceneName = (SceneId == DefaultSceneId) ? "DefaultScene" : ("Scene_" + std::to_string(SceneId));
        SceneMap.insert(std::make_pair(Scene->SceneId, Scene));
    }

    SFUInt32 OldSceneId = GetActorSceneId(ActorId);
    if (OldSceneId != 0 && OldSceneId != SceneId)
    {
        RemoveActorFromScene(OldSceneId, ActorId);
    }

    Scene->AddActor(ActorId);
    ActorSceneMap[ActorId] = SceneId;
    return true;
}

bool SFModule_SceneManager::RemoveActorFromScene(SFUInt32 SceneId, SFActorId ActorId)
{
    SFScene *Scene = FindScene(SceneId);
    if (Scene == nullptr)
    {
        return false;
    }

    if (!Scene->HasActor(ActorId))
    {
        return false;
    }

    Scene->RemoveActor(ActorId);
    ActorSceneMap.erase(ActorId);

    if (NetworkServer != nullptr)
    {
        std::vector<SFUInt32> SessionIds = GetScenePlayerSessionIds(SceneId);
        std::vector<SFUInt32> OtherSessionIds;
        SFUInt32 LeavingSessionId = (ActorFactory != nullptr) ? ActorFactory->GetActorSessionId(ActorId) : 0;
        for (SFUInt32 SessionId : SessionIds)
        {
            if (SessionId != 0 && SessionId != LeavingSessionId)
            {
                OtherSessionIds.push_back(SessionId);
            }
        }

        char Payload[4] = {};
        SFUInt32 NetActorId = htonl(ActorId);
        std::memcpy(Payload, &NetActorId, 4);
        NetworkServer->BroadcastTo(OtherSessionIds, SF_MSGID_ACTOR_LEAVE_SCENE, Payload, 4);
    }

    return true;
}

std::vector<SFActorId> SFModule_SceneManager::GetSceneActors(SFUInt32 SceneId)
{
    SFScene *Scene = FindScene(SceneId);
    if (Scene == nullptr)
    {
        return {};
    }
    return Scene->GetActorIds();
}

std::vector<SFUInt32> SFModule_SceneManager::GetScenePlayerSessionIds(SFUInt32 SceneId)
{
    std::vector<SFUInt32> SessionIds;
    if (ActorFactory == nullptr)
    {
        return SessionIds;
    }

    std::vector<SFActorId> Actors = GetSceneActors(SceneId);
    for (SFActorId ActorId : Actors)
    {
        if (ActorFactory->GetActorType(ActorId) == ESFActorType::Player)
        {
            SFUInt32 SessionId = ActorFactory->GetActorSessionId(ActorId);
            if (SessionId != 0)
            {
                SessionIds.push_back(SessionId);
            }
        }
    }

    return SessionIds;
}

SFUInt32 SFModule_SceneManager::GetActorSceneId(SFActorId ActorId)
{
    auto Iter = ActorSceneMap.find(ActorId);
    if (Iter == ActorSceneMap.end())
    {
        return 0;
    }
    return Iter->second;
}

#pragma endregion ISFSceneManager

ISFNetworkServer *SFModule_SceneManager::FindNetworkServer()
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

    SFString PluginName = "Skywalker::Framework::SFPlugin_Network";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_NetworkServer";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFNetworkServer *>(Module);
}

ISFActorFactory *SFModule_SceneManager::FindActorFactory()
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

    SFString PluginName = "Skywalker::Framework::SFPlugin_Actor";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_ActorFactory";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFActorFactory *>(Module);
}

SFScene *SFModule_SceneManager::FindScene(SFUInt32 SceneId)
{
    auto Iter = SceneMap.find(SceneId);
    if (Iter == SceneMap.end())
    {
        return nullptr;
    }
    return Iter->second;
}

void SFModule_SceneManager::CleanupScenes()
{
    SF_COMMON_ITERATOR(IterScene, SceneMap)
    {
        if (IterScene->second != nullptr)
        {
            delete IterScene->second;
        }
    }
    SceneMap.clear();
}

void SFModule_SceneManager::HandleEnterSceneReq(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
{
    if (NetworkServer == nullptr || ActorFactory == nullptr)
    {
        return;
    }

    if (PayloadLen < 4)
    {
        return;
    }

    SFUInt32 NetSceneId = 0;
    std::memcpy(&NetSceneId, Payload, 4);
    SFUInt32 SceneId = ntohl(NetSceneId);

    SFUInt32 PlayerId = NetworkServer->GetSessionPlayerId(SessionId);
    SFActorId PlayerActorId = ActorFactory->FindActorByPlayerId(PlayerId);
    if (PlayerActorId == 0)
    {
        return;
    }

    if (!AddActorToScene(SceneId, PlayerActorId))
    {
        return;
    }

    std::vector<SFUInt32> SessionIds = GetScenePlayerSessionIds(SceneId);
    std::vector<SFUInt32> OtherSessionIds;
    for (SFUInt32 Sid : SessionIds)
    {
        if (Sid != SessionId)
        {
            OtherSessionIds.push_back(Sid);
        }
    }

    char EnterPayload[SF_ACTOR_INFO_SIZE] = {};
    if (ActorFactory->SerializeActorInfo(PlayerActorId, EnterPayload, sizeof(EnterPayload)) == SF_ACTOR_INFO_SIZE)
    {
        NetworkServer->BroadcastTo(OtherSessionIds, SF_MSGID_ACTOR_ENTER_SCENE,
                                   EnterPayload, static_cast<SFUInt32>(sizeof(EnterPayload)));
    }

    std::vector<SFActorId> Actors = GetSceneActors(SceneId);
    SFUInt16 ActorCount = static_cast<SFUInt16>(Actors.size());
    SFUInt32 RespLen = 1 + 2 + static_cast<SFUInt32>(ActorCount) * static_cast<SFUInt32>(SF_ACTOR_INFO_SIZE);
    std::vector<char> Resp(RespLen, 0);
    Resp[0] = 0;
    SFUInt16 NetCount = htons(ActorCount);
    std::memcpy(Resp.data() + 1, &NetCount, 2);

    SFUInt32 Offset = 3;
    for (SFActorId ActorId : Actors)
    {
        SFSize Wrote = ActorFactory->SerializeActorInfo(ActorId, Resp.data() + Offset, SF_ACTOR_INFO_SIZE);
        if (Wrote == SF_ACTOR_INFO_SIZE)
        {
            Offset += static_cast<SFUInt32>(Wrote);
        }
    }

    NetworkServer->SendTo(SessionId, SF_MSGID_ENTER_SCENE_RESP, Resp.data(), Offset);
}

void SFModule_SceneManager::HandleMoveReq(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
{
    if (NetworkServer == nullptr || ActorFactory == nullptr)
    {
        return;
    }

    if (PayloadLen < 12)
    {
        return;
    }

    SFUInt32 PlayerId = NetworkServer->GetSessionPlayerId(SessionId);
    SFActorId ActorId = ActorFactory->FindActorByPlayerId(PlayerId);
    if (ActorId == 0)
    {
        return;
    }

    SFVector3 Pos;
    std::memcpy(&Pos.X, Payload, 4);
    std::memcpy(&Pos.Y, Payload + 4, 4);
    std::memcpy(&Pos.Z, Payload + 8, 4);
    ActorFactory->SetActorPosition(ActorId, Pos);

    SFUInt32 SceneId = GetActorSceneId(ActorId);
    if (SceneId == 0)
    {
        return;
    }

    std::vector<SFUInt32> SessionIds = GetScenePlayerSessionIds(SceneId);
    std::vector<SFUInt32> OtherSessionIds;
    for (SFUInt32 Sid : SessionIds)
    {
        if (Sid != SessionId)
        {
            OtherSessionIds.push_back(Sid);
        }
    }

    char MovePayload[16] = {};
    SFUInt32 NetActorId = htonl(ActorId);
    std::memcpy(MovePayload, &NetActorId, 4);
    std::memcpy(MovePayload + 4, &Pos.X, 4);
    std::memcpy(MovePayload + 8, &Pos.Y, 4);
    std::memcpy(MovePayload + 12, &Pos.Z, 4);
    NetworkServer->BroadcastTo(OtherSessionIds, SF_MSGID_MOVE_BROADCAST, MovePayload, 16);
}
