/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SSFModule_ReplayRecorder.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 回放录制模块
*************************************************************************/

#include "SSFModule_ReplayRecorder.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_ReplayRecorder, ESFLogLevel::Debug);

void SSFModule_ReplayRecorder::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("ReplayRecorder module init");
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
    return true;
}

bool SSFModule_ReplayRecorder::StopRecord()
{
    if (bRecording == FALSE)
    {
        return false;
    }

    bRecording = FALSE;
    RecordingSessionId = 0;
    return true;
}

bool SSFModule_ReplayRecorder::IsRecording() const
{
    return bRecording == TRUE;
}
