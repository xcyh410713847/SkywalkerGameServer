/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SSFModule_ReplayRecorder.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 回放录制模块
*************************************************************************/

#include "SSFModule_ReplayRecorder.h"

#include "Include/SFILog.h"

#include "Core/Service/SSFGameplayServiceGateway.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include <filesystem>
#include <fstream>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_ReplayRecorder, ESFLogLevel::Debug);

static const SFString GReplayMagic = "SKYWALKER_REPLAY";
static const SFUInt64 GReplayFormatVersion = 2;
static const SFUInt64 GReplayChecksumOffset = 1469598103934665603ULL;
static const SFUInt64 GReplayChecksumPrime = 1099511628211ULL;

void SSFModule_ReplayRecorder::Init(SFObjectErrors &Errors)
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

    SF_LOG_FRAMEWORK("ReplayRecorder module init");
}

void SSFModule_ReplayRecorder::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    SSFGameplayServiceGateway::Instance().RegisterReplayStartRecord(
        [this](SFUInt64 SessionId)
        {
            return StartRecord(SessionId);
        });

    SSFGameplayServiceGateway::Instance().RegisterReplayStopRecord(
        [this]()
        {
            return StopRecord();
        });

    SSFGameplayServiceGateway::Instance().RegisterReplayRecordStats(
        [this]()
        {
            return BuildStats();
        });
}

void SSFModule_ReplayRecorder::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterReplayStartRecord(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayStopRecord(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayRecordStats(nullptr);

    StopRecord();

    SSFModule::Stop(Errors);
}

void SSFModule_ReplayRecorder::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("ReplayRecorder module destroy");
    SSFModule::Destroy(Errors);
}

bool SSFModule_ReplayRecorder::StartRecord(SFUInt64 SessionId)
{
    if (SessionId == 0 || bRecording == TRUE)
    {
        return false;
    }

    bRecording = TRUE;
    RecordingSessionId = SessionId;
    RecordingFilePath = BuildReplayFilePath(SessionId);
    RecordingEvents.clear();

    SF_LOG_FRAMEWORK("ReplayRecorder start record, SessionId " << SessionId << " File " << RecordingFilePath);
    return true;
}

bool SSFModule_ReplayRecorder::RecordEvent(const SFString &EventLine)
{
    if (bRecording == FALSE || EventLine.empty())
    {
        return false;
    }

    RecordingEvents.push_back(EventLine);
    return true;
}

bool SSFModule_ReplayRecorder::StopRecord()
{
    if (bRecording == FALSE)
    {
        return false;
    }

    std::filesystem::path PathObj(RecordingFilePath);
    if (PathObj.has_parent_path())
    {
        std::filesystem::create_directories(PathObj.parent_path());
    }

    std::ofstream OutFile(RecordingFilePath, std::ios::trunc);
    if (!OutFile.is_open())
    {
        SF_LOG_ERROR("ReplayRecorder stop record failed, File " << RecordingFilePath);
        return false;
    }

    OutFile << "Magic=" << GReplayMagic << "\n";
    OutFile << "Version=" << GReplayFormatVersion << "\n";
    OutFile << "SessionId=" << RecordingSessionId << "\n";
    OutFile << "EventCount=" << RecordingEvents.size() << "\n";
    OutFile << "Checksum=" << BuildChecksum() << "\n";
    OutFile << "EventsBegin" << "\n";

    for (const auto &Line : RecordingEvents)
    {
        OutFile << Line << "\n";
    }

    LastRecordedSessionId = RecordingSessionId;
    LastRecordedEventCount = static_cast<SFUInt64>(RecordingEvents.size());
    LastRecordedChecksum = BuildChecksum();
    LastRecordedFilePath = RecordingFilePath;

    SF_LOG_FRAMEWORK("ReplayRecorder stop record, SessionId " << RecordingSessionId
                                                              << " EventCount " << RecordingEvents.size()
                                                              << " File " << RecordingFilePath);

    bRecording = FALSE;
    RecordingSessionId = 0;
    RecordingFilePath.clear();
    RecordingEvents.clear();
    return true;
}

bool SSFModule_ReplayRecorder::IsRecording() const
{
    return bRecording == TRUE;
}

void SSFModule_ReplayRecorder::SetReplayDirectory(const SFString &InReplayDirectory)
{
    if (!InReplayDirectory.empty())
    {
        ReplayDirectory = InReplayDirectory;
    }
}

SFString SSFModule_ReplayRecorder::BuildStats() const
{
    return "Recording=" + std::to_string(static_cast<SFUInt64>(bRecording)) +
           ";RecordingSessionId=" + std::to_string(RecordingSessionId) +
           ";LastRecordedSessionId=" + std::to_string(LastRecordedSessionId) +
           ";LastRecordedEventCount=" + std::to_string(LastRecordedEventCount) +
           ";LastRecordedChecksum=" + std::to_string(LastRecordedChecksum) +
           ";ReplayDirectory=" + ReplayDirectory +
           ";LastRecordedFilePath=" + LastRecordedFilePath;
}

SFUInt64 SSFModule_ReplayRecorder::BuildChecksum() const
{
    SFUInt64 Checksum = GReplayChecksumOffset;
    for (const auto &Line : RecordingEvents)
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

SFString SSFModule_ReplayRecorder::BuildReplayFilePath(SFUInt64 SessionId) const
{
    return ReplayDirectory + "/Session_" + std::to_string(SessionId) + ".replay";
}
