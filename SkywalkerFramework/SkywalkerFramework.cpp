/*************************************************************************
**文件: SkywalkerFramework\SkywalkerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerFramework
*************************************************************************/

#include "SkywalkerFramework.h"

#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Service/FrameworkService/SSFService_Timer.h"
#include "Core/Service/FrameworkService/SSFService_Event.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SkywalkerFramework, LogLevel_Debug);

SSF_SHARED_PTR(SkywalkerFramework)
SSFFramework = SSF_MAKE_SHARED_PTR(CSkywalkerFramework);

bool CSkywalkerFramework::Start()
{
    // 打印地址
    SSF_LOG_DEBUG_MODULE("Start");

    // 进入启动中状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Starting;

    ServiceManager = NewObject<SSFFrameworkServiceManager>();

    SSF_LOG_DEBUG_OBJECT(ServiceManager, "Create");

    // 时间服务
    auto TimerService = GetService<SSFService_Timer>();

    SSF_LOG_INFO("Start Time: " << TimerService->GetCurrTimeStr())

    // 事件服务
    auto EventService = GetService<SSFService_Event>();

    // 插件管理器
    PluginManager = NewObject<SSFPluginManager>();

    // Init
    SSFObjectErrors ObjectErrors;
    PluginManager->Init(ObjectErrors);

    // Awake
    PluginManager->Awake(ObjectErrors);

    // Start
    PluginManager->Start(ObjectErrors);

    // 进入运行状态
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Running;

    SSF_LOG_INFO("Start Finish, Elapsed Time: " << TimerService->GetDeltaTime() << "ms");

    return true;
}

bool CSkywalkerFramework::Tick()
{
    if (!IsRunning())
    {
        return false;
    }

    SSFObjectErrors ObjectErrors;

    ServiceManager->Tick(ObjectErrors);

    PluginManager->Tick(ObjectErrors, GetService<SSFService_Timer>()->GetDeltaTime());

    return true;
}

bool CSkywalkerFramework::Stop()
{
    auto TimerService = GetService<SSFService_Timer>();

    SSFObjectErrors ObjectErrors;

    // Stop
    PluginManager->Stop(ObjectErrors);

    // Sleep
    PluginManager->Sleep(ObjectErrors);

    // Destroy
    PluginManager->Destroy(ObjectErrors);

    // Release
    PluginManager->Release(ObjectErrors);
    PluginManager = nullptr;

    SSF_LOG_INFO("Stop Time: " << TimerService->GetCurrTimeStr() << " ; Elapsed Time: " << TimerService->GetTotalTime() << "s");

    ServiceManager->Release(ObjectErrors);
    ServiceManager = nullptr;

    return true;
}

void CSkywalkerFramework::Close()
{
    RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Stopping;
}

bool CSkywalkerFramework::IsRunning() const
{
    return RunningState == ERunningState::SkywalkerServerFrameworkRunningState_Running;
}

SSFObjectGUID CSkywalkerFramework::NewObjectGUID()
{
    AddObjectGUID++;
    return AddObjectGUID;
}
