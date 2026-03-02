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
}

void SSFModule_ReplayRecorder::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterReplayStartRecord(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterReplayStopRecord(nullptr);

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
    RecordingEvents.push_back("SessionId=" + std::to_string(SessionId));

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

    for (const auto &Line : RecordingEvents)
    {
        OutFile << Line << "\n";
    }

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

SFString SSFModule_ReplayRecorder::BuildReplayFilePath(SFUInt64 SessionId) const
{
    return ReplayDirectory + "/Session_" + std::to_string(SessionId) + ".replay";
}
