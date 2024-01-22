/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFramework.h
**作者: shyfan
**日期: 2024/01/03 16:49:50
**功能: SkywalkerServerFramework
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFramework_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFramework_H__

#include "Include/SSFCore.h"

SSF_NAMESPACE_BEGIN

class SSFPluginManager;

// Skywalker Server Framework 运行状态
enum class ESkywalkerServerFrameworkRunningState
{
    SkywalkerServerFrameworkRunningState_Create = 0, // 创建
    SkywalkerServerFrameworkRunningState_Starting,   // 启动中
    SkywalkerServerFrameworkRunningState_Running,    // 运行中
    SkywalkerServerFrameworkRunningState_Stopping,   // 停止中
};

class SkywalkerServerFramework
{
public:
    virtual bool Start() = 0;
    virtual bool Tick() = 0;
    virtual bool Stop() = 0;

    /**
     * 关闭
     */
    virtual void Close() = 0;

    /**
     * 是否正在运行
     */
    virtual bool IsRunning() const = 0;

    /**
     * new一个ObjectGUID
    */
    virtual SSFObjectGUID NewObjectGUID() = 0;

    /**
     * 获取插件管理器
     */
    virtual SSF_SHARED_PTR(SSFPluginManager) GetPluginManager() const = 0;
};

SSF_NAMESPACE_END

/**
 * 全局 Skywalker Server Framework
 */
extern SSF_SHARED_PTR(SSF_NAMESPACE::SkywalkerServerFramework) SSFramework;

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

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFramework_H__
