/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SFModule_ReplayPlayer.h
**作者: shyfan
**日期: 2026/03/02
**功能: 回放播放模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYPLAYER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYPLAYER_H__

#include "Core/Module/SFModule.h"

#include <vector>

SF_NAMESPACE_BEGIN

class SFModule_ReplayPlayer : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_ReplayPlayer(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_ReplayPlayer() {};

    bool StartReplay(SFUInt64 SessionId);
    bool StopReplay();
    bool IsReplaying() const;
    void SetReplayDirectory(const SFString &InReplayDirectory);
    SFUInt64 GetLoadedEventCount() const;
    SFString BuildStats() const;
    SFString GetEventByIndex(SFUInt64 EventIndex);
    SFString GetEventsRange(SFUInt64 StartIndex, SFUInt64 Count);
    SFString FindEventsByKeyword(const SFString &Keyword, SFUInt64 MaxCount);

private:
    SFUInt64 BuildChecksum() const;
    SFString BuildReplayFilePath(SFUInt64 SessionId) const;

private:
    SFBool bReplaying = FALSE;
    SFUInt64 ReplaySessionId = 0;
    SFString ReplayDirectory = "Replay";
    std::vector<SFString> LoadedEvents;
    SFUInt64 ReplayFormatVersion = 0;
    SFUInt64 LastReplaySessionId = 0;
    SFUInt64 LastLoadedEventCount = 0;
    SFUInt64 ReplayHeaderChecksum = 0;
    SFUInt64 ReplayVerifiedChecksum = 0;
    SFUInt64 LastQueryEventIndex = static_cast<SFUInt64>(-1);
    SFString LastQueryKeyword;
    SFUInt64 LastQueryKeywordHits = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYPLAYER_H__
