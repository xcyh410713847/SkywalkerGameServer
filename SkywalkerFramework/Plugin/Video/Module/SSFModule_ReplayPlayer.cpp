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

static const SFString GReplayMagic = "SKYWALKER_REPLAY";
static const SFUInt64 GReplayFormatVersion = 2;
static const SFUInt64 GReplayChecksumOffset = 1469598103934665603ULL;
static const SFUInt64 GReplayChecksumPrime = 1099511628211ULL;

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

    SSFGameplayServiceGateway::Instance().RegisterReplayPlayStats(
        [this]()
        {
            return BuildStats();
        });
}

void SSFModule_ReplayPlayer::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterReplayStartPlay(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayStopPlay(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayPlayStats(nullptr);

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
    ReplayFormatVersion = 0;
    ReplayHeaderChecksum = 0;
    ReplayVerifiedChecksum = 0;

    SFString MagicLine;
    SFString VersionLine;
    SFString SessionLine;
    SFString EventCountLine;
    SFString ChecksumLine;
    SFString EventsBeginLine;
    if (!std::getline(InFile, MagicLine) ||
        !std::getline(InFile, VersionLine) ||
        !std::getline(InFile, SessionLine) ||
        !std::getline(InFile, EventCountLine) ||
        !std::getline(InFile, ChecksumLine) ||
        !std::getline(InFile, EventsBeginLine))
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, invalid replay header " << ReplayFilePath);
        return false;
    }

    if (MagicLine != ("Magic=" + GReplayMagic))
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, invalid replay magic " << ReplayFilePath);
        return false;
    }

    const SFString VersionPrefix = "Version=";
    if (VersionLine.rfind(VersionPrefix, 0) != 0)
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, invalid replay version header " << ReplayFilePath);
        return false;
    }

    ReplayFormatVersion = std::stoull(VersionLine.substr(VersionPrefix.size()));
    if (ReplayFormatVersion != GReplayFormatVersion)
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, unsupported replay version " << ReplayFormatVersion);
        return false;
    }

    const SFString ChecksumPrefix = "Checksum=";
    if (ChecksumLine.rfind(ChecksumPrefix, 0) != 0)
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, invalid replay checksum header " << ReplayFilePath);
        return false;
    }

    ReplayHeaderChecksum = std::stoull(ChecksumLine.substr(ChecksumPrefix.size()));

    if (EventsBeginLine != "EventsBegin")
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, missing EventsBegin " << ReplayFilePath);
        return false;
    }

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

    ReplayVerifiedChecksum = BuildChecksum();
    if (ReplayHeaderChecksum != ReplayVerifiedChecksum)
    {
        SF_LOG_ERROR("ReplayPlayer start replay failed, checksum mismatch HeaderChecksum " << ReplayHeaderChecksum
                                                                                             << " VerifiedChecksum " << ReplayVerifiedChecksum);
        return false;
    }

    bReplaying = TRUE;
    ReplaySessionId = SessionId;
    LastReplaySessionId = SessionId;
    LastLoadedEventCount = static_cast<SFUInt64>(LoadedEvents.size());
    SF_LOG_FRAMEWORK("ReplayPlayer start replay, SessionId " << SessionId
                                                             << " EventCount " << LoadedEvents.size()
                                                             << " FormatVersion " << ReplayFormatVersion
                                                             << " Checksum " << ReplayVerifiedChecksum);
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

SFString SSFModule_ReplayPlayer::BuildStats() const
{
    return "Replaying=" + std::to_string(static_cast<SFUInt64>(bReplaying)) +
           ";ReplaySessionId=" + std::to_string(ReplaySessionId) +
           ";LastReplaySessionId=" + std::to_string(LastReplaySessionId) +
           ";LoadedEventCount=" + std::to_string(static_cast<SFUInt64>(LoadedEvents.size())) +
           ";LastLoadedEventCount=" + std::to_string(LastLoadedEventCount) +
           ";ReplayFormatVersion=" + std::to_string(ReplayFormatVersion) +
           ";ReplayHeaderChecksum=" + std::to_string(ReplayHeaderChecksum) +
           ";ReplayVerifiedChecksum=" + std::to_string(ReplayVerifiedChecksum) +
           ";ReplayDirectory=" + ReplayDirectory;
}

SFUInt64 SSFModule_ReplayPlayer::BuildChecksum() const
{
    SFUInt64 Checksum = GReplayChecksumOffset;
    for (const auto &Line : LoadedEvents)
    {
        for (const auto &Ch : Line)
        {
            Checksum ^= static_cast<SFUInt64>(static_cast<unsigned char>(Ch));
            Checksum *= GReplayChecksumPrime;
        }

        Checksum ^= static_cast<SFUInt64>('\n');
        Checksum *= GReplayChecksumPrime;
    }

    return Checksum;
}

SFString SSFModule_ReplayPlayer::BuildReplayFilePath(SFUInt64 SessionId) const
{
    return ReplayDirectory + "/Session_" + std::to_string(SessionId) + ".replay";
}
