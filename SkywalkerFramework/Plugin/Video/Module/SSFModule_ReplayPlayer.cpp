/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SSFModule_ReplayPlayer.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 回放播放模块
*************************************************************************/

#include "SSFModule_ReplayPlayer.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_ReplayPlayer, ESFLogLevel::Debug);

void SSFModule_ReplayPlayer::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("ReplayPlayer module init");
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

    bReplaying = TRUE;
    ReplaySessionId = SessionId;
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
    return true;
}

bool SSFModule_ReplayPlayer::IsReplaying() const
{
    return bReplaying == TRUE;
}
