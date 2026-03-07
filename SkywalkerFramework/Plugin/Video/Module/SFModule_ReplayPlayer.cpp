/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SFModule_ReplayPlayer.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 回放播放模块
*************************************************************************/

#include "SFModule_ReplayPlayer.h"

#include "Include/SFILog.h"

#include "Core/Service/SFGameplayServiceGateway.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include <fstream>
#include <sstream>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_ReplayPlayer, ESFLogLevel::Debug);

static const SFString GReplayMagic = "SKYWALKER_REPLAY";
static const SFUInt64 GReplayFormatVersion = 2;
static const SFUInt64 GReplayChecksumOffset = 1469598103934665603ULL;
static const SFUInt64 GReplayChecksumPrime = 1099511628211ULL;

void SFModule_ReplayPlayer::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, SF_ENV_PROGRAM_CONFIG_DIR);
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv(SF_ENV_PROGRAM_CONFIG_DIR);
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

void SFModule_ReplayPlayer::Start(SFObjectErrors &Errors)
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

    SSFGameplayServiceGateway::Instance().RegisterReplayGetEventByIndex(
        [this](SFUInt64 EventIndex)
        {
            return GetEventByIndex(EventIndex);
        });

    SSFGameplayServiceGateway::Instance().RegisterReplayGetEventsRange(
        [this](SFUInt64 StartIndex, SFUInt64 Count)
        {
            return GetEventsRange(StartIndex, Count);
        });

    SSFGameplayServiceGateway::Instance().RegisterReplayFindEventsByKeyword(
        [this](const SFString &Keyword, SFUInt64 MaxCount)
        {
            return FindEventsByKeyword(Keyword, MaxCount);
        });
}

void SFModule_ReplayPlayer::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterReplayStartPlay(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayStopPlay(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayPlayStats(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayGetEventByIndex(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayGetEventsRange(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayFindEventsByKeyword(nullptr);

    StopReplay();

    SSFModule::Stop(Errors);
}

void SFModule_ReplayPlayer::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("ReplayPlayer module destroy");
    SSFModule::Destroy(Errors);
}

bool SFModule_ReplayPlayer::StartReplay(SFUInt64 SessionId)
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

bool SFModule_ReplayPlayer::StopReplay()
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

bool SFModule_ReplayPlayer::IsReplaying() const
{
    return bReplaying == TRUE;
}

void SFModule_ReplayPlayer::SetReplayDirectory(const SFString &InReplayDirectory)
{
    if (!InReplayDirectory.empty())
    {
        ReplayDirectory = InReplayDirectory;
    }
}

SFUInt64 SFModule_ReplayPlayer::GetLoadedEventCount() const
{
    return static_cast<SFUInt64>(LoadedEvents.size());
}

SFString SFModule_ReplayPlayer::BuildStats() const
{
    return "Replaying=" + std::to_string(static_cast<SFUInt64>(bReplaying)) +
           ";ReplaySessionId=" + std::to_string(ReplaySessionId) +
           ";LastReplaySessionId=" + std::to_string(LastReplaySessionId) +
           ";LoadedEventCount=" + std::to_string(static_cast<SFUInt64>(LoadedEvents.size())) +
           ";LastLoadedEventCount=" + std::to_string(LastLoadedEventCount) +
           ";LastQueryEventIndex=" + std::to_string(LastQueryEventIndex) +
           ";LastQueryKeyword=" + LastQueryKeyword +
           ";LastQueryKeywordHits=" + std::to_string(LastQueryKeywordHits) +
           ";ReplayFormatVersion=" + std::to_string(ReplayFormatVersion) +
           ";ReplayHeaderChecksum=" + std::to_string(ReplayHeaderChecksum) +
           ";ReplayVerifiedChecksum=" + std::to_string(ReplayVerifiedChecksum) +
           ";ReplayDirectory=" + ReplayDirectory;
}

SFString SFModule_ReplayPlayer::GetEventByIndex(SFUInt64 EventIndex)
{
    LastQueryEventIndex = EventIndex;
    if (EventIndex >= LoadedEvents.size())
    {
        return "";
    }

    return "EventIndex=" + std::to_string(EventIndex) + ";Event=" + LoadedEvents[static_cast<size_t>(EventIndex)];
}

SFString SFModule_ReplayPlayer::GetEventsRange(SFUInt64 StartIndex, SFUInt64 Count)
{
    if (Count == 0 || StartIndex >= LoadedEvents.size())
    {
        return "";
    }

    const SFUInt64 EndIndexExclusive = std::min<SFUInt64>(static_cast<SFUInt64>(LoadedEvents.size()), StartIndex + Count);

    std::ostringstream Stream;
    Stream << "EventRange=";
    for (SFUInt64 Index = StartIndex; Index < EndIndexExclusive; ++Index)
    {
        if (Index > StartIndex)
        {
            Stream << "|";
        }

        Stream << "EventIndex=" << Index << ";Event=" << LoadedEvents[static_cast<size_t>(Index)];
    }

    return Stream.str();
}

SFString SFModule_ReplayPlayer::FindEventsByKeyword(const SFString &Keyword, SFUInt64 MaxCount)
{
    LastQueryKeyword = Keyword;
    LastQueryKeywordHits = 0;
    if (Keyword.empty() || MaxCount == 0)
    {
        return "";
    }

    std::ostringstream Stream;
    Stream << "EventSearch=";
    bool bHasResult = false;
    for (size_t Index = 0; Index < LoadedEvents.size(); ++Index)
    {
        if (LoadedEvents[Index].find(Keyword) == SFString::npos)
        {
            continue;
        }

        if (bHasResult)
        {
            Stream << "|";
        }

        Stream << "EventIndex=" << Index << ";Event=" << LoadedEvents[Index];
        bHasResult = true;
        ++LastQueryKeywordHits;
        if (LastQueryKeywordHits >= MaxCount)
        {
            break;
        }
    }

    if (!bHasResult)
    {
        return "";
    }

    return Stream.str();
}

SFUInt64 SFModule_ReplayPlayer::BuildChecksum() const
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

SFString SFModule_ReplayPlayer::BuildReplayFilePath(SFUInt64 SessionId) const
{
    return ReplayDirectory + "/Session_" + std::to_string(SessionId) + ".replay";
}
