/*************************************************************************
**文件: SkywalkerFramework\SkywalkerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerFramework
*************************************************************************/

#include "SkywalkerFramework.h"

#include "Include/SFILog.h"

#include "Core/Plugin/SFPluginManager.h"
#include "Core/Service/FrameworkService/SFService_Timer.h"
#include "Core/Service/FrameworkService/SFService_Event.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SkywalkerFramework, ESFLogLevel::Debug);

/**
 * 全局框架实例
 * 说明：进程级唯一实例，负责整个服务端框架生命周期。
 */
SF_UNIQUE_PTR(SkywalkerFramework)
SFFramework = SF_MAKE_UNIQUE_PTR(CSkywalkerFramework);

/**
 * 启动框架
 * 流程：创建服务管理器 -> 初始化关键服务 -> 创建并启动插件管理器 -> 进入运行态。
 */
bool CSkywalkerFramework::Start()
{
    // 输出模块启动日志
    SF_LOG_DEBUG_MODULE("Start");

    // 切换到启动中状态
    RunningState = ERunningState::RunningState_Starting;

    // 创建框架服务管理器（内部负责基础服务注册/调度）
    ServiceManager = NewObject<SSFFrameworkServiceManager>();

    SF_LOG_DEBUG_OBJECT(ServiceManager, "Create");

    // 获取时间服务，用于记录启动时间和后续帧间隔
    auto TimerService = GetService<SSFService_Timer>();

    SF_LOG_INFO("Start Time: " << TimerService->GetCurrTimeStr())

    // 获取事件服务（确保事件系统在启动阶段就绪）
    auto EventService = GetService<SSFService_Event>();

    // 创建插件管理器
    PluginManager = NewObject<SFPluginManager>();

    // 插件生命周期阶段：Init
    SFObjectErrors ObjectErrors;
    PluginManager->Init(ObjectErrors);
    if (ObjectErrors.IsValid())
    {
        auto FirstError = ObjectErrors.GetFirstError();
        SF_LOG_ERROR("PluginManager Init Failed: " << FirstError.GetContent())
        RunningState = ERunningState::RunningState_Create;
        return false;
    }

    // 插件生命周期阶段：Awake
    PluginManager->Awake(ObjectErrors);
    if (ObjectErrors.IsValid())
    {
        auto FirstError = ObjectErrors.GetFirstError();
        SF_LOG_ERROR("PluginManager Awake Failed: " << FirstError.GetContent())
        RunningState = ERunningState::RunningState_Create;
        return false;
    }

    // 插件生命周期阶段：Start
    PluginManager->Start(ObjectErrors);
    if (ObjectErrors.IsValid())
    {
        auto FirstError = ObjectErrors.GetFirstError();
        SF_LOG_ERROR("PluginManager Start Failed: " << FirstError.GetContent())
        RunningState = ERunningState::RunningState_Create;
        return false;
    }

    // 启动完成，切换到运行状态
    RunningState = ERunningState::RunningState_Running;

    SF_LOG_INFO("Start Finish, Elapsed Time: " << TimerService->GetDeltaTime() << "ms");

    return true;
}

/**
 * 框架主循环
 * 说明：先驱动服务，再驱动插件；当框架不在运行态时立即结束循环。
 */
bool CSkywalkerFramework::Tick()
{
    if (!IsRunning())
    {
        return false;
    }

    SFObjectErrors ObjectErrors;

    ServiceManager->Tick(ObjectErrors);

    // 使用时间服务的帧间隔驱动插件 Tick
    PluginManager->Tick(ObjectErrors, GetService<SSFService_Timer>()->GetDeltaTime());

    return true;
}

/**
 * 停止框架
 * 流程：插件 Stop -> Sleep -> Destroy -> Release，随后释放服务管理器。
 */
bool CSkywalkerFramework::Stop()
{
    SFObjectErrors ObjectErrors;

    // 插件生命周期阶段：Stop（停止业务逻辑）
    PluginManager->Stop(ObjectErrors);

    // 插件生命周期阶段：Sleep（进入休眠/挂起态）
    PluginManager->Sleep(ObjectErrors);

    // 插件生命周期阶段：Destroy（释放内部资源）
    PluginManager->Destroy(ObjectErrors);

    // 释放插件管理器对象
    PluginManager->Release(ObjectErrors);
    PluginManager = nullptr;

    auto TimerService = GetService<SSFService_Timer>();
    SF_LOG_INFO("Stop Time: " << TimerService->GetCurrTimeStr() << " ; Elapsed Time: " << TimerService->GetTotalTime() << "s");

    ServiceManager->Release(ObjectErrors);
    ServiceManager = nullptr;

    if (ObjectErrors.IsValid())
    {
        auto FirstError = ObjectErrors.GetFirstError();
        SF_LOG_ERROR("Stop Error: " << FirstError.GetContent())
    }

    return true;
}

/**
 * 请求关闭框架
 * 说明：仅修改状态为 Stopping，主循环将据此退出。
 */
void CSkywalkerFramework::Close()
{
    RunningState = ERunningState::RunningState_Stopping;
}

/**
 * 判断框架是否处于运行态
 */
bool CSkywalkerFramework::IsRunning() const
{
    return RunningState == ERunningState::RunningState_Running;
}

/**
 * 生成递增对象 GUID
 */
SFObjectGUID CSkywalkerFramework::NewObjectGUID()
{
    AddObjectGUID++;
    return AddObjectGUID;
}
