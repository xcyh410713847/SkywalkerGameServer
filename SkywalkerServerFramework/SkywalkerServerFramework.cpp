﻿/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerServerFramework
*************************************************************************/

#include "SkywalkerServerFramework.h"

#include "Include/SSFILog.h"

// #define SKYWALKER_SF_START_AND_STOP

SSF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SkywalkerServerFramework, LogLevel_Debug);

void CSkywalkerServerFramework::SignalHandler(int Signal)
{

    SKYWALKER_SF_LOG_DEBUG("SignalHandler Signal: " << Signal);

    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stopping;
}

bool CSkywalkerServerFramework::Start()
{
    // 进入启动中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Starting;

    FrameworkTimer.Reset();

    SKYWALKER_SF_LOG_INFO("SkywalkerServerFramework Start Time: " << FrameworkTimer.GetStartTime() << "s")

    // 创建插件管理器
    PluginManager = SSF_NEW_OBJECT(SSFOPluginManager);

    // Init
    SSFObjectErrors ObjectErrors;
    PluginManager->Init(ObjectErrors);

    // Awake
    PluginManager->Awake(ObjectErrors);

    // Start
    PluginManager->Start(ObjectErrors);

    // 进入运行状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Running;

    signal(SIGINT, &CSkywalkerServerFramework::SignalHandler);

    FrameworkTimer.Tick();
    SKYWALKER_SF_LOG_INFO("SkywalkerServerFramework Start Finish, Elapsed Time: " << FrameworkTimer.GetDeltaTime() << "ms");

    return true;
}

bool CSkywalkerServerFramework::Tick()
{
#ifdef SKYWALKER_SF_START_AND_STOP
    return false;
#endif // SKYWALKER_SF_START_AND_STOP

    if (!IsRunning())
    {
        return false;
    }

    FrameworkTimer.Tick();

    SSFObjectErrors ObjectErrors;
    PluginManager->Tick(ObjectErrors, FrameworkTimer.GetDeltaTime());

    return true;
}

bool CSkywalkerServerFramework::Stop()
{
    SKYWALKER_SF_LOG_INFO("SkywalkerServerFramework Stop Begin");

    SSFObjectErrors ObjectErrors;

    // Stop
    PluginManager->Stop(ObjectErrors);

    // Sleep
    PluginManager->Sleep(ObjectErrors);

    // Destroy
    PluginManager->Destroy(ObjectErrors);

    // Release
    PluginManager->Release(ObjectErrors);

    // 进入已停止状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stoped;

    SKYWALKER_SF_LOG_INFO("SkywalkerServerFramework Stop Time: " << FrameworkTimer.GetCurrTime()
                                                                 << "s, Elapsed Time: " << FrameworkTimer.GetTotalTime() << "s");

    return true;
}

bool CSkywalkerServerFramework::IsRunning() const
{
    return RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Running;
}
