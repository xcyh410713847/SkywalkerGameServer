/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerServerFramework
*************************************************************************/

#include "SkywalkerServerFramework.h"

#include "Include/SSFILog.h"

// #define SKYWALKER_SF_START_AND_STOP

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SkywalkerServerFramework, LogLevel_Debug);

void CSkywalkerServerFramework::SignalHandler(int Signal)
{
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stopping;
}

bool CSkywalkerServerFramework::Start()
{
    SKYWALKER_SF_LOG_INFO("SkywalkerServerFramework Start Begin");

    // 进入启动中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Starting;

    // 创建插件管理器
    PluginManager = NewObject<SSFOPluginManager>();

    // Init
    SSFObjectErrors ObjectErrors;
    PluginManager->Init(ObjectErrors);

    // Awake
    PluginManager->Awake(ObjectErrors);

    // Start
    PluginManager->Start(ObjectErrors);

    // 进入运行状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Running;

    SKYWALKER_SF_LOG_INFO("SkywalkerServerFramework Start Finish");

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

    // TODO Shyfan 临时
    SKYWALKER_PLATFORM_SLEEP(1000);

    SSFObjectErrors ObjectErrors;
    PluginManager->Tick(ObjectErrors, 1000);

    signal(SIGINT, &CSkywalkerServerFramework::SignalHandler);

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

    SKYWALKER_SF_LOG_INFO("SkywalkerServerFramework Stop Finish");

    return true;
}

bool CSkywalkerServerFramework::IsRunning() const
{
    return RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Running;
}
