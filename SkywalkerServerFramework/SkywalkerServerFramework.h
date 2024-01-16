/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.h
**作者: shyfan
**日期: 2023/08/08 10:07:38
**功能: SkywalkerServerFramework
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_H__

#include "Include/SSFCore.h"
#include "Include/SSFramework.h"

#include "SkywalkerTimer/SkywalkerTimer.h"
#include "SkywalkerPool/SkywalkerPool.h"

#include "Core/Object/SSFObject.h"

SSF_NAMESPACE_BEGIN

/**
 * Skywalker Server Framework
 */
class CSkywalkerServerFramework : public SkywalkerServerFramework
{
    typedef ESkywalkerServerFrameworkRunningState ERunningState;

#pragma region SkywalkerServerFramework

public:
    virtual bool Start() override;
    virtual bool Tick() override;
    virtual bool Stop() override;

    /**
     * 关闭
     */
    virtual void Close() override;

    /**
     * 是否正在运行
     */
    virtual bool IsRunning() const override;

    /**
     * 获取插件管理器
     */
    virtual SSF_SHARED_PTR(SSFOPluginManager) GetPluginManager() const override
    {
        return PluginManager;
    }

#pragma endregion SkywalkerServerFramework

#pragma region Object

public:
    /**
     * 创建对象
     */
    template <typename T>
    SSF_SHARED_PTR(T)
    NewSharedObject();

    /**
     * 创建对象
     */
    template <typename T>
    SSF_SHARED_PTR(T)
    NewSharedObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors);

    /**
     * 创建对象
     */
    template <typename T>
    SSF_PTR(T)
    NewObject();

    /**
     * 创建对象
     */
    template <typename T>
    SSF_PTR(T)
    NewObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors);

private:
    SSFMap<std::string, SKYWALKER_POOL_PTR(SSFObject)> ObjectPoolMap;

#pragma endregion Object

private:
    SSF_SHARED_PTR(SSFOPluginManager)
    PluginManager{};

    SSF_SHARED_PTR(SKYWALKER_TIMER_NAMESPACE::SkywalkerTimer)
    FrameworkTimer{};

    ERunningState RunningState{ERunningState::SkywalkerServerFrameworkRunningState_Create};
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
