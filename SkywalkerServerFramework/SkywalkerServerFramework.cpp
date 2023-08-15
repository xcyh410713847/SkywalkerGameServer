/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerServerFramework
*************************************************************************/

#include "SkywalkerServerFramework.h"

#include <iostream>
#include <csignal>

SKYWALKER_SF_NAMESPACE_USE

SKYWALKER_SINGLETON_IMPLEMENT(CSkywalkerServerFramework);

void CSkywalkerServerFramework::SignalHandler(int Signal)
{
    std::cout << "Received signal: " << Signal << std::endl;
}

bool CSkywalkerServerFramework::Start()
{
    std::cout << "Hello SkywalkerServerFramework" << std::endl;

    // 进入启动中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Starting;

    // 插件管理器初始化
    SSFCPluginManager *PluginManager = SSFCPluginManager::GetInstance();

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

    std::cout << "Tick SkywalkerServerFramework" << std::endl;

    // TODO Shyfan 临时
    SKYWALKER_PLATFORM_SLEEP(1000);

    SSFObjectErrors ObjectErrors;
    SSFCPluginManager::GetInstance()->Tick(ObjectErrors, 1000);

    signal(SIGINT, &CSkywalkerServerFramework::SignalHandler);

    return true;
}

void CSkywalkerServerFramework::Stop()
{
    std::cout << "Bye SkywalkerServerFramework" << std::endl;

    // 进入停止中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stopping;

    SSFObjectErrors ObjectErrors;

    // Stop
    SSFCPluginManager::GetInstance()->Stop(ObjectErrors);

    // Destroy
    SSFCPluginManager::GetInstance()->Destroy(ObjectErrors);

    // Release
    SSFCPluginManager::GetInstance()->Release(ObjectErrors);

    // 进入已停止状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stoped;
}

bool CSkywalkerServerFramework::IsRunning() const
{
    return RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Running ||
           RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Pausing;
}
