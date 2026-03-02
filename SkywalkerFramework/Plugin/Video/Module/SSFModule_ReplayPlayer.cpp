/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SSFModule_ReplayPlayer.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 回放播放模块
*************************************************************************/

#include "SSFModule_ReplayPlayer.h"

#include "Include/SFILog.h"

#include "Core/Service/SSFGameplayServiceGateway.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include <fstream>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_ReplayPlayer, ESFLogLevel::Debug);

void SSFModule_ReplayPlayer::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, "SKYWALKER_SERVER_CONFIG");
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv("SKYWALKER_SERVER_CONFIG");
#endif
    SFString ServerConfigPath = ConfigPath ? ConfigPath : "ServerConfig.skywalkerC";
#if defined(_WIN32) || defined(_WIN64)
    if (ConfigPathBuffer != nullptr)
    {
        free(ConfigPathBuffer);
        ConfigPathBuffer = nullptr;
    }
#endif

    SKYWALKER_PTR_SCRIPT_PARSE ConfigParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (ConfigParse->LoadScript(ServerConfigPath.c_str()))
    {
        SKYWALKER_PTR_SCRIPT_NODE RootNode = ConfigParse->GetRootNode();
        if (RootNode != nullptr)
        {
            for (size_t i = 0; i < RootNode->GetChildNodeNum(); i++)
            {
                SKYWALKER_PTR_SCRIPT_NODE ConfigNode = RootNode->GetChildNodeFromIndex(i);
                if (ConfigNode == nullptr)
                {
                    continue;
                }

                SKYWALKER_PTR_SCRIPT_NODE ReplayDirNode = ConfigNode->GetChildNodeFromName("ReplayDirectory");
                if (ReplayDirNode != nullptr)
                {
                    SetReplayDirectory(ReplayDirNode->GetNodeValueString());
                }
            }
        }
    }

    SF_LOG_FRAMEWORK("ReplayPlayer module init");
}

void SSFModule_ReplayPlayer::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    SSFGameplayServiceGateway::Instance().RegisterReplayStartPlay(
        [this](SFUInt64 SessionId)
        {
            return StartReplay(SessionId);
        });

    SSFGameplayServiceGateway::Instance().RegisterReplayStopPlay(
        [this]()
        {
            return StopReplay();
        });
}

void SSFModule_ReplayPlayer::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterReplayStartPlay(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayStopPlay(nullptr);

    StopReplay();

    SSFModule::Stop(Errors);
}

void SSFModule_ReplayPlayer::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("ReplayPlayer module destroy");
    SSFModule::Destroy(Errors);
}

bool SSFModule_ReplayPlayer::StartReplay(SFUInt64 SessionId)
{
    if (SessionId == 0 || bReplaying == TRUE)
    {
        return false;
    }

    SFString ReplayFilePath = BuildReplayFilePath(SessionId);
    std::ifstream InFile(ReplayFilePath);
    if (!InFile.is_open())
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, File " << ReplayFilePath);
        return false;
    }

    LoadedEvents.clear();
    SFString Line;
    while (std::getline(InFile, Line))
    {
        if (!Line.empty())
        {
            LoadedEvents.push_back(Line);
        }
    }

    if (LoadedEvents.empty())
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, empty replay file " << ReplayFilePath);
        return false;
    }

    bReplaying = TRUE;
    ReplaySessionId = SessionId;
    SF_LOG_FRAMEWORK("ReplayPlayer start replay, SessionId " << SessionId
                                                              << " EventCount " << LoadedEvents.size());
    return true;
}

bool SSFModule_ReplayPlayer::StopReplay()
{
    if (bReplaying == FALSE)
    {
        return false;
    }

    bReplaying = FALSE;
    ReplaySessionId = 0;
    LoadedEvents.clear();
    return true;
}

bool SSFModule_ReplayPlayer::IsReplaying() const
{
    return bReplaying == TRUE;
}

void SSFModule_ReplayPlayer::SetReplayDirectory(const SFString &InReplayDirectory)
{
    if (!InReplayDirectory.empty())
    {
        ReplayDirectory = InReplayDirectory;
    }
}

SFUInt64 SSFModule_ReplayPlayer::GetLoadedEventCount() const
{
    return static_cast<SFUInt64>(LoadedEvents.size());
}

SFString SSFModule_ReplayPlayer::BuildReplayFilePath(SFUInt64 SessionId) const
{
    return ReplayDirectory + "/Session_" + std::to_string(SessionId) + ".replay";
}
