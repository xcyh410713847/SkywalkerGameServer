/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerServerFramework
*************************************************************************/

#include "SkywalkerServerFramework.h"

#include "Include/SSFILog.h"

#include "Core/Command/SSFCommandLine.h"
#include "Core/Plugin/SSFPluginManager.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SkywalkerServerFramework, LogLevel_Debug);

SSF_SHARED_PTR(SkywalkerServerFramework)
SSFramework = SSF_MAKE_SHARED_PTR(CSkywalkerServerFramework);

bool CSkywalkerServerFramework::Start()
{
    // 打印地址
    SSF_LOG_DEBUG("SkywalkerServerFramework Start Begin, Address: " << this);

    // 进入启动中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Starting;

    FrameworkTimer = SSF_MAKE_SHARED_PTR(SKYWALKER_TIMER_NAMESPACE::SkywalkerTimer);
    if (!FrameworkTimer)
    {
        SSF_LOG_ERROR("SkywalkerServerFramework Start Failed, Create SkywalkerTimer Failed");
        return false;
    }

    FrameworkTimer->Reset();

    SSF_LOG_INFO("SkywalkerServerFramework Start Time: " << FrameworkTimer->GetStartTime() << "s")

    CommandLine = SSF_MAKE_SHARED_PTR(SSFCommandLine);
    if (!CommandLine)
    {
        SSF_LOG_ERROR("SkywalkerServerFramework Start Failed, Create SSFCommandLine Failed");
        return false;
    }

    // 创建插件管理器
    PluginManager = SSF_NEW_SHARED_OBJECT(SSFOPluginManager);
    if (!PluginManager)
    {
        SSF_LOG_ERROR("SkywalkerServerFramework Start Failed, Create SSFOPluginManager Failed");
        return false;
    }

    // Init
    SSFObjectErrors ObjectErrors;
    PluginManager->Init(ObjectErrors);

    // Awake
    PluginManager->Awake(ObjectErrors);

    // Start
    PluginManager->Start(ObjectErrors);

    // 进入运行状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Running;

    FrameworkTimer->Tick();
    SSF_LOG_INFO("SkywalkerServerFramework Start Finish, Elapsed Time: " << FrameworkTimer->GetDeltaTime() << "ms");

    return true;
}

bool CSkywalkerServerFramework::Tick()
{
    if (!IsRunning())
    {
        return false;
    }

    FrameworkTimer->Tick();

    SSFObjectErrors ObjectErrors;
    PluginManager->Tick(ObjectErrors, FrameworkTimer->GetDeltaTime());

    return true;
}

bool CSkywalkerServerFramework::Stop()
{
    SSF_LOG_INFO("SkywalkerServerFramework Stop Begin, Address: " << this);

    SSFObjectErrors ObjectErrors;

    // Stop
    PluginManager->Stop(ObjectErrors);

    // Sleep
    PluginManager->Sleep(ObjectErrors);

    // Destroy
    PluginManager->Destroy(ObjectErrors);

    // Release
    PluginManager->Release(ObjectErrors);

    SSF_LOG_INFO("SkywalkerServerFramework Stop Time: " << FrameworkTimer->GetCurrTime()
                                                        << "s, Elapsed Time: " << FrameworkTimer->GetTotalTime() << "s");

    PluginManager = nullptr;
    FrameworkTimer = nullptr;
    CommandLine = nullptr;

    return true;
}

void CSkywalkerServerFramework::Close()
{
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stopping;
}

bool CSkywalkerServerFramework::IsRunning() const
{
    return RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Running;
}
