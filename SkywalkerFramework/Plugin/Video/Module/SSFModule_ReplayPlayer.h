/*************************************************************************
**文件: SkywalkerFramework\Plugin\Video\Module\SSFModule_ReplayPlayer.h
**作者: shyfan
**日期: 2026/03/02
**功能: 回放播放模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYPLAYER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYPLAYER_H__

#include "Core/Module/SSFModule.h"

SF_NAMESPACE_BEGIN

class SSFModule_ReplayPlayer : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_ReplayPlayer(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_ReplayPlayer() {};

    bool StartReplay(SFUInt64 SessionId);
    bool StopReplay();
    bool IsReplaying() const;

private:
    SFBool bReplaying = FALSE;
    SFUInt64 ReplaySessionId = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_REPLAYPLAYER_H__
