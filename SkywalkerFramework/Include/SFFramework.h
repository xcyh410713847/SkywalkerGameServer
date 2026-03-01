/*************************************************************************
**文件: SkywalkerFramework\Include\SFFramework.h
**作者: shyfan
**日期: 2024/01/03 16:49:50
**功能: SkywalkerFramework
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFFRAMEWORK_H__
#define __SKYWALKER_FRAMEWORK_SFFRAMEWORK_H__

#include "Include/SFCore.h"

SF_NAMESPACE_BEGIN

class SFPluginManager;

// Skywalker Framework 运行状态
enum class ESkywalkerFrameworkRunningState
{
    SkywalkerFrameworkRunningState_Create = 0, // 创建
    SkywalkerFrameworkRunningState_Starting,   // 启动中
    SkywalkerFrameworkRunningState_Running,    // 运行中
    SkywalkerFrameworkRunningState_Stopping,   // 停止中
};

class SkywalkerFramework
{
public:
    /**
     * 启动框架
     */
    virtual bool Start() = 0;

    /**
     * 框架主循环
     */
    virtual bool Tick() = 0;

    /**
     * 停止框架
     */
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
    virtual SFObjectGUID NewObjectGUID() = 0;

    /**
     * 获取插件管理器
     */
    virtual SF_PTR(SFPluginManager) GetPluginManager() const = 0;
};

SF_NAMESPACE_END

/**
 * 全局 Skywalker Framework
 */
extern SF_SHARED_PTR(SF_NAMESPACE::SkywalkerFramework) SFFramework;

/**
 * Skywalker Framework 启动宏
 */
#define SKYWALKER_FRAMEWORK_START(argc, argv) \
    if (!SFFramework->Start())                \
    {                                         \
        return 1;                             \
    }                                         \
    while (SFFramework->Tick())               \
    {                                         \
    }                                         \
    SFFramework->Stop();

#endif //__SKYWALKER_FRAMEWORK_SFFRAMEWORK_H__
