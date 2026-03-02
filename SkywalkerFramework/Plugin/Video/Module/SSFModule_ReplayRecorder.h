/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SSFModule_ReplayRecorder.h
**作者: shyfan
**日期: 2026/03/02
**功能: 回放录制模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYRECORDER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYRECORDER_H__

#include "Core/Module/SSFModule.h"

#include <vector>

SF_NAMESPACE_BEGIN

class SSFModule_ReplayRecorder : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_ReplayRecorder(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_ReplayRecorder() {};

    bool StartRecord(SFUInt64 SessionId);
    bool RecordEvent(const SFString &EventLine);
    bool StopRecord();
    bool IsRecording() const;
    void SetReplayDirectory(const SFString &InReplayDirectory);

private:
    SFString BuildReplayFilePath(SFUInt64 SessionId) const;

private:
    SFBool bRecording = FALSE;
    SFUInt64 RecordingSessionId = 0;
    SFString ReplayDirectory = "Replay";
    SFString RecordingFilePath;
    std::vector<SFString> RecordingEvents;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYRECORDER_H__
