/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.cpp
**作者: shyfan
**日期: 2023/08/08 10:07:45
**功能: SkywalkerServerFramework
*************************************************************************/

#include "SkywalkerServerFramework.h"

#include "Include/SSFILog.h"

#include "Core/Plugin/SSFPluginManager.h"
#include "Core/Service/FrameworkService/SSFService_Event.h"
#include "Core/Service/SSFLevelService.h"

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

    ServiceManager = NewObject<SSFServiceManager<SSFFrameworkService>>();

    // 创建插件管理器
    PluginManager = NewSharedObject<SSFPluginManager>();
    if (!PluginManager)
    {
        SSF_LOG_ERROR("SkywalkerServerFramework Start Failed, Create SSFPluginManager Failed");
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

SSFObjectGUID CSkywalkerServerFramework::NewObjectGUID()
{
    AddObjectGUID++;
    return AddObjectGUID;
}

#pragma region NewObject

/**
 * 创建对象
 */
template <typename T>
SSF_SHARED_PTR(T)
CSkywalkerServerFramework::NewSharedObject()
{
    // TODO Shyfan 判断是否是继承自SSFObject
    SSFObjectContext InContext;
    InContext.SSFramework = this;
    SSFObjectErrors InErrors;
    return NewSharedObject<T>(InContext, InErrors);
}

/**
 * 创建对象
 */
template <typename T>
SSF_SHARED_PTR(T)
CSkywalkerServerFramework::NewSharedObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
{
    // TODO Shyfan 判断是否是继承自SSFObject
    SSF_PTR(T)
    Object = NewObject<T>(InContext, InErrors);

    std::shared_ptr<T> SharedObject(Object);

    return SharedObject;
}

/**
 * 创建对象
 */
template <typename T>
SSF_PTR(T)
CSkywalkerServerFramework::NewObject()
{
    // TODO Shyfan 判断是否是继承自SSFObject
    SSFObjectContext InContext;
    InContext.SSFramework = this;
    SSFObjectErrors InErrors;

    return NewObject<T>(InContext, InErrors);
}

/**
 * 创建对象
 */
template <typename T>
SSF_PTR(T)
CSkywalkerServerFramework::NewObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
{
    // TODO Shyfan 判断是否是继承自SSFObject
    // 从对象池中获取对象
    std::string ClassName{};
    SSF_CLASS_NAME(T, ClassName);
    auto Iterator = ObjectPoolMap.find(ClassName);
    if (Iterator != ObjectPoolMap.end())
    {
        SKYWALKER_POOL_PTR(SSFObject)
        ObjectPool = Iterator->second;
        if (SSF_PTR_VALID(ObjectPool))
        {
            SSFObject *Object = ObjectPool->Get();
            if (SSF_PTR_VALID(Object))
            {
                return (T *)Object;
            }
        }
    }

    return new T(InContext, InErrors);
}

#pragma endregion NewObject
