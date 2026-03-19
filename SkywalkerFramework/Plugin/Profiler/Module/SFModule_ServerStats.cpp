/*************************************************************************
**文件: SkywalkerFramework\Plugin\Profiler\Module\SFModule_ServerStats.cpp
**作者: shyfan
**日期: 2026/03/19
**功能: 服务器统计监控模块
*************************************************************************/

#include "SFModule_ServerStats.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFNetworkInterface.h"
#include "Include/SFActorTypes.h"

#include "Core/Plugin/SFPlugin.h"
#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_ServerStats, Framework);

#pragma region Object

void SFModule_ServerStats::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SFModule_ServerStats::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);

    NetworkServer = FindNetworkServer();
    SceneManager = FindSceneManager();

    if (NetworkServer == nullptr)
    {
        SF_LOG_ERROR("ServerStats cannot find NetworkServer interface");
    }
    if (SceneManager == nullptr)
    {
        SF_LOG_ERROR("ServerStats cannot find SceneManager interface");
    }

    SF_LOG_FRAMEWORK("ServerStats initialized, report interval=" << ReportIntervalMS << "ms");
}

void SFModule_ServerStats::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);
}

void SFModule_ServerStats::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    ElapsedMS += DelayMS;
    if (ElapsedMS >= ReportIntervalMS)
    {
        PrintStats();
        ElapsedMS = 0;
    }
}

void SFModule_ServerStats::Stop(SFObjectErrors &Errors)
{
    SSFModule::Stop(Errors);
}

void SFModule_ServerStats::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SFModule_ServerStats::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

ISFNetworkServer *SFModule_ServerStats::FindNetworkServer()
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

ISFSceneManager *SFModule_ServerStats::FindSceneManager()
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

void SFModule_ServerStats::PrintStats()
{
    SFUInt32 SessionCount = 0;
    SFUInt32 OnlinePlayerCount = 0;
    SFUInt64 RecvMsgCount = 0;
    SFUInt64 SendMsgCount = 0;

    if (NetworkServer != nullptr)
    {
        SessionCount = NetworkServer->GetSessionCount();
        OnlinePlayerCount = NetworkServer->GetAuthenticatedSessionCount();
        RecvMsgCount = NetworkServer->GetTotalRecvMsgCount();
        SendMsgCount = NetworkServer->GetTotalSendMsgCount();
        NetworkServer->ResetMsgCounters();
    }

    SFUInt32 SceneActorCount = 0;
    if (SceneManager != nullptr)
    {
        SFUInt32 DefaultSceneId = SceneManager->GetDefaultSceneId();
        std::vector<SFActorId> SceneActors = SceneManager->GetSceneActors(DefaultSceneId);
        SceneActorCount = static_cast<SFUInt32>(SceneActors.size());
    }

    float IntervalSec = static_cast<float>(ReportIntervalMS) / 1000.0f;
    float RecvPerSec = (IntervalSec > 0.0f) ? static_cast<float>(RecvMsgCount) / IntervalSec : 0.0f;
    float SendPerSec = (IntervalSec > 0.0f) ? static_cast<float>(SendMsgCount) / IntervalSec : 0.0f;

    SF_LOG_FRAMEWORK("[Stats] Sessions=" << SessionCount
                     << " Players=" << OnlinePlayerCount
                     << " SceneActors=" << SceneActorCount
                     << " MsgRecv=" << RecvMsgCount << "(" << static_cast<int>(RecvPerSec) << "/s)"
                     << " MsgSend=" << SendMsgCount << "(" << static_cast<int>(SendPerSec) << "/s)");
}
