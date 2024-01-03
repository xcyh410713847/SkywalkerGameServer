/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.h
**作者: shyfan
**日期: 2023/08/08 10:07:38
**功能: SkywalkerServerFramework
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_H__

#include "Include/SSFCore.h"

#include "SkywalkerTimer/SkywalkerTimer.h"

SSF_NAMESPACE_BEGIN

class SSFCommandLine;
class SSFOPluginManager;

// Skywalker Server Framework 运行状态
enum class ESkywalkerServerFrameworkRunningState
{
    SkywalkerServerFrameworkRunningState_Create = 0, // 创建
    SkywalkerServerFrameworkRunningState_Starting,   // 启动中
    SkywalkerServerFrameworkRunningState_Running,    // 运行中
    SkywalkerServerFrameworkRunningState_Stopping,   // 停止中
};

/**
 * Skywalker Server Framework
 */
class CSkywalkerServerFramework
{
    typedef ESkywalkerServerFrameworkRunningState ERunningState;

public:
    bool Start();
    bool Tick();
    bool Stop();

    /**
     * 关闭
     */
    void Close();

    /**
     * 是否正在运行
     */
    bool IsRunning() const;

    SSF_SHARED_PTR(SSFOPluginManager)
    GetPluginManager() const
    {
        return PluginManager;
    }

private:
    SSF_SHARED_PTR(SSFOPluginManager)
    PluginManager{};

    SSF_SHARED_PTR(SKYWALKER_TIMER_NAMESPACE::SkywalkerTimer)
    FrameworkTimer{};

    SSF_SHARED_PTR(SSFCommandLine)
    CommandLine{};

    ERunningState RunningState{ERunningState::SkywalkerServerFrameworkRunningState_Create};
};

SSF_NAMESPACE_END

/**
 * 全局 Skywalker Server Framework
 */
extern SSF_UNIQUE_PTR(SSF_NAMESPACE::CSkywalkerServerFramework) SSFramework;

/**
 * Skywalker Server Framework 启动宏
 */
#define SKYWALKER_SERVER_FRAMEWORK_START(argc, argv) \
    if (!SSFramework->Start())                       \
    {                                                \
        return 1;                                    \
    }                                                \
    while (SSFramework->Tick())                      \
    {                                                \
    }                                                \
    SSFramework->Stop();

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
