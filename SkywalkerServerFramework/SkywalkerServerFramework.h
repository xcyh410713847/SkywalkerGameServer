/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.h
**作者: shyfan
**日期: 2023/08/08 10:07:38
**功能: SkywalkerServerFramework
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_H__

#include <csignal>

#include "Include/SSFCore.h"

#include "SkywalkerTimer/SkywalkerTimer.h"

#include "Core/Command/SSFCommandLine.h"
#include "Core/Plugin/SSFPluginManager.h"

SSF_NAMESPACE_BEGIN

// Skywalker Server Framework 运行状态
enum class ESkywalkerServerFrameworkRunningState
{
    SkywalkerServerFrameworkRunningState_Create = 0, // 创建
    SkywalkerServerFrameworkRunningState_Starting,   // 启动中
    SkywalkerServerFrameworkRunningState_Running,    // 运行中
    SkywalkerServerFrameworkRunningState_Stopping,   // 停止中
    SkywalkerServerFrameworkRunningState_Stoped,     // 已停止
};

/**
 * 运行状态
 */
typedef ESkywalkerServerFrameworkRunningState ERunningState;
static ERunningState RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Create;

/**
 * Skywalker Server Framework
 */
class CSkywalkerServerFramework
{
public:
    bool Start();
    bool Tick();
    bool Stop();

    bool IsRunning() const;

private:
    SSF_PTR_PLUGIN_MANAGER PluginManager = nullptr;

    SKYWALKER_TIMER_NAMESPACE::SkywalkerTimer FrameworkTimer;

    SSFCommandLine CommandLine;
};

SSF_NAMESPACE_END

/**
 * Skywalker Server Framework 启动宏
 */
#define SKYWALKER_SERVER_FRAMEWORK_START(argc, argv) \
    SSF_UNIQUE_PTR(CSkywalkerServerFramework)        \
    SSFramework(new CSkywalkerServerFramework());    \
    if (!SSFramework->Start())                       \
    {                                                \
        return 1;                                    \
    }                                                \
    while (SSFramework->Tick())                      \
    {                                                \
    }                                                \
    SSFramework->Stop();

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
