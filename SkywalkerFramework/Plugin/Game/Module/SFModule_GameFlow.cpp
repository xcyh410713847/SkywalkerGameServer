/*************************************************************************
**文件: SkywalkerFramework\Plugin\Game\Module\SFModule_GameFlow.cpp
**作者: shyfan
**日期: 2026/03/19
**功能: 游戏流程模块
*************************************************************************/

#include "SFModule_GameFlow.h"

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

SF_LOG_DEFINE(SFModule_GameFlow, Framework);

#pragma region Object

void SFModule_GameFlow::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SFModule_GameFlow::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);

    NetworkServer = FindNetworkServer();
    ActorFactory = FindActorFactory();
    SceneManager = FindSceneManager();
    if (NetworkServer == nullptr || ActorFactory == nullptr || SceneManager == nullptr)
    {
        SF_LOG_ERROR("GameFlow dependencies missing");
        return;
    }

    NetworkServer->RegisterHandler(SF_MSGID_PLAYER_ENTER,
        [this](SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
        {
            HandlePlayerEnter(SessionId, Payload, PayloadLen);
        });

    NetworkServer->RegisterHandler(SF_MSGID_PLAYER_LEAVE,
        [this](SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
        {
            HandlePlayerLeave(SessionId, Payload, PayloadLen);
        });
}

void SFModule_GameFlow::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);
    SpawnInitialWorldActors();
}

void SFModule_GameFlow::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
}

void SFModule_GameFlow::Stop(SFObjectErrors &Errors)
{
    if (NetworkServer != nullptr)
    {
        NetworkServer->UnregisterHandler(SF_MSGID_PLAYER_ENTER);
        NetworkServer->UnregisterHandler(SF_MSGID_PLAYER_LEAVE);
    }

    SSFModule::Stop(Errors);
}

void SFModule_GameFlow::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_GameFlow::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

ISFNetworkServer *SFModule_GameFlow::FindNetworkServer()
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

ISFActorFactory *SFModule_GameFlow::FindActorFactory()
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

ISFSceneManager *SFModule_GameFlow::FindSceneManager()
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

    SFString PluginName = "Skywalker::Framework::SFPlugin_Level";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_SceneManager";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFSceneManager *>(Module);
}

void SFModule_GameFlow::SpawnInitialWorldActors()
{
    if (ActorFactory == nullptr || SceneManager == nullptr)
    {
        return;
    }

    SFUInt32 SceneId = SceneManager->GetDefaultSceneId();

    SFActorId NPC1 = ActorFactory->CreateNPC(1, SFVector3(10.0f, 0.0f, 0.0f),
                                             {SFVector3(10.0f, 0.0f, 0.0f),
                                              SFVector3(20.0f, 0.0f, 0.0f),
                                              SFVector3(20.0f, 0.0f, 10.0f),
                                              SFVector3(10.0f, 0.0f, 10.0f)});
    SceneManager->AddActorToScene(SceneId, NPC1);

    SFActorId NPC2 = ActorFactory->CreateNPC(2, SFVector3(30.0f, 0.0f, 0.0f),
                                             {SFVector3(30.0f, 0.0f, 0.0f),
                                              SFVector3(40.0f, 0.0f, 0.0f)});
    SceneManager->AddActorToScene(SceneId, NPC2);

    SFActorId Monster1 = ActorFactory->CreateMonster(1, SFVector3(50.0f, 0.0f, 50.0f), 15.0f);
    SceneManager->AddActorToScene(SceneId, Monster1);

    SFActorId Monster2 = ActorFactory->CreateMonster(2, SFVector3(60.0f, 0.0f, 30.0f), 20.0f);
    SceneManager->AddActorToScene(SceneId, Monster2);

    SF_LOG_FRAMEWORK("GameFlow spawned default NPCs and monsters in SceneId=" << SceneId);
}

void SFModule_GameFlow::HandlePlayerEnter(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
{
    if (ActorFactory == nullptr || SceneManager == nullptr)
    {
        return;
    }

    if (PayloadLen < 4)
    {
        return;
    }

    SFUInt32 NetPlayerId = 0;
    std::memcpy(&NetPlayerId, Payload, 4);
    SFUInt32 PlayerId = ntohl(NetPlayerId);

    if (ActorFactory->FindActorByPlayerId(PlayerId) != 0)
    {
        return;
    }

    SFString PlayerName = "Player_" + std::to_string(PlayerId);
    SFActorId ActorId = ActorFactory->CreatePlayer(PlayerId, SessionId, PlayerName);
    SceneManager->AddActorToScene(SceneManager->GetDefaultSceneId(), ActorId);

    SF_LOG_FRAMEWORK("GameFlow created player actor PlayerId=" << PlayerId
                     << " ActorId=" << ActorId
                     << " SessionId=" << SessionId);
}

void SFModule_GameFlow::HandlePlayerLeave(SFUInt32 SessionId, const char *Payload, SFUInt32 PayloadLen)
{
    if (ActorFactory == nullptr || SceneManager == nullptr)
    {
        return;
    }

    if (PayloadLen < 4)
    {
        return;
    }

    SFUInt32 NetPlayerId = 0;
    std::memcpy(&NetPlayerId, Payload, 4);
    SFUInt32 PlayerId = ntohl(NetPlayerId);

    SFActorId ActorId = ActorFactory->FindActorByPlayerId(PlayerId);
    if (ActorId == 0)
    {
        return;
    }

    SFUInt32 SceneId = SceneManager->GetActorSceneId(ActorId);
    if (SceneId != 0)
    {
        SceneManager->RemoveActorFromScene(SceneId, ActorId);
    }

    ActorFactory->DestroyActor(ActorId);

    SF_LOG_FRAMEWORK("GameFlow removed player actor PlayerId=" << PlayerId
                     << " ActorId=" << ActorId
                     << " SessionId=" << SessionId);
}
