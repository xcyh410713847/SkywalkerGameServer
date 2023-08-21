/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerServerFramework
*************************************************************************/

#include "SkywalkerServerFramework.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SF_LOG_DEFINE(SkywalkerServerFramework, LogLevel_Debug);

SKYWALKER_SINGLETON_IMPLEMENT(CSkywalkerServerFramework);

void CSkywalkerServerFramework::SignalHandler(int Signal)
{
    SKYWALKER_SF_LOG_DEBUG("Received signal: " << Signal);
}

bool CSkywalkerServerFramework::Start()
{
    SKYWALKER_SF_LOG_INFO("Start SkywalkerServerFramework ");

    // 进入启动中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Starting;

    // 插件管理器初始化
    SSFOPluginManager *PluginManager = SSFOPluginManager::GetInstance();

    // Init
    SSFObjectErrors ObjectErrors;
    PluginManager->Init(ObjectErrors);

    // Awake
    PluginManager->Awake(ObjectErrors);

    // Start
    PluginManager->Start(ObjectErrors);

    // 进入运行状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Running;

    return true;
}

bool CSkywalkerServerFramework::Tick()
{
    if (!IsRunning())
    {
        return false;
    }

    // TODO Shyfan 临时
    SKYWALKER_PLATFORM_SLEEP(1000);

    SSFObjectErrors ObjectErrors;
    SSFOPluginManager::GetInstance()->Tick(ObjectErrors, 1000);

    signal(SIGINT, &CSkywalkerServerFramework::SignalHandler);

    return true;
}

void CSkywalkerServerFramework::Stop()
{
    SKYWALKER_SF_LOG_INFO("Stop SkywalkerServerFramework");

    // 进入停止中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stopping;

    SSFObjectErrors ObjectErrors;

    // Stop
    SSFOPluginManager::GetInstance()->Stop(ObjectErrors);

    // Destroy
    SSFOPluginManager::GetInstance()->Destroy(ObjectErrors);

    // Release
    SSFOPluginManager::GetInstance()->Release(ObjectErrors);

    // 进入已停止状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stoped;
}

bool CSkywalkerServerFramework::IsRunning() const
{
    return RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Running ||
           RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Pausing;
}
