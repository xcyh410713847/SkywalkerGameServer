/*************************************************************************
**文件: SkywalkerServerFramework\SkywalkerServerFramework.h
**作者: shyfan
**日期: 2023/08/08 10:07:38
**功能: SkywalkerServerFramework
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_H__

#include "SkywalkerSingleton\SkywalkerSingleton.h"

#include "Include\SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

// Skywalker Server Framework 运行状态
enum class ESkywalkerServerFrameworkRunningState
{
    SkywalkerServerFrameworkRunningState_Create = 0, // 创建
    SkywalkerServerFrameworkRunningState_Starting,   // 启动中
    SkywalkerServerFrameworkRunningState_Running,    // 运行中
    SkywalkerServerFrameworkRunningState_Pausing,    // 暂停中
    SkywalkerServerFrameworkRunningState_Stopping,   // 停止中
    SkywalkerServerFrameworkRunningState_Stoped,     // 已停止
};

class CSkywalkerServerFramework
{
    SKYWALKER_SINGLETON_DECLARE(CSkywalkerServerFramework);

public:
    static void SignalHandler(int Signal);

public:
    bool Start();
    bool Tick();
    void Stop();

    bool IsRunning() const;

private:
    typedef ESkywalkerServerFrameworkRunningState ERunningState;
    ERunningState RunningState = ERunningState::SkywalkerServerFrameworkRunningState_Create;
};

SKYWALKER_SF_NAMESPACE_END

#define SKYWALKER_SERVER_FRAMEWORK_START(argc, argv)                                                 \
    CSkywalkerServerFramework *pSkywalkerServerFramework = CSkywalkerServerFramework::GetInstance(); \
    if (!pSkywalkerServerFramework->Start())                                                         \
    {                                                                                                \
        return 1;                                                                                    \
    }                                                                                                \
    while (pSkywalkerServerFramework->IsRunning())                                                   \
    {                                                                                                \
        pSkywalkerServerFramework->Tick();                                                           \
    }                                                                                                \
    pSkywalkerServerFramework->Stop();                                                               \
    pSkywalkerServerFramework->DestroyInstance();                                                    \
    return 0;

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
