/*************************************************************************
**文件: SkywalkerFramework\SkywalkerFramework.h
**作者: shyfan
**日期: 2023/08/08 10:07:38
**功能: SkywalkerFramework
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_H__

#include "Include/SFCore.h"
#include "Include/SFFramework.h"

#include "Core/Object/SFObject.h"
#include "Core/Service/SFServiceManager.h"

SF_NAMESPACE_BEGIN

/**
 * Skywalker Framework 运行状态枚举
 */
enum class ESFRunningState
{
    RunningState_Create = 0, // 已创建（尚未启动）
    RunningState_Starting,   // 启动流程中
    RunningState_Running,    // 正常运行中
    RunningState_Stopping,   // 停止流程中
};

/**
 * SkywalkerFramework 主体实现类
 *
 * 职责：
 * 1. 管理框架生命周期（Start/Tick/Stop/Close）
 * 2. 维护插件管理器与服务管理器
 * 3. 统一分配对象 GUID
 */
class CSkywalkerFramework : public SkywalkerFramework, public SSFObject
{
    typedef ESFRunningState ERunningState;

public:
    /** 构造函数 */
    CSkywalkerFramework() {};
    /** 析构函数 */
    virtual ~CSkywalkerFramework() {};

    /**
     * 获取指定类型服务
     * @tparam ServiceT 服务类型
     * @return 服务指针；若服务未注册则返回 nullptr
     */
    template <typename ServiceT>
    SF_PTR(ServiceT)
    GetService() const
    {
        return ServiceManager->GetService<ServiceT>();
    }

    /**
     * 获取指定类型插件
     * @tparam PluginT 插件类型
     * @return 插件指针；若插件未注册则返回 nullptr
     */
    template <typename PluginT>
    SF_PTR(PluginT)
    GetPlugin() const
    {
        return PluginManager->GetPlugin<PluginT>();
    }

#pragma region SkywalkerFramework

public:
    /**
     * 启动框架
     * @return true 启动成功，false 启动失败
     */
    virtual bool Start() override;

    /**
     * 框架主循环 Tick
     * @return true 继续运行，false 需要结束运行
     */
    virtual bool Tick() override;

    /**
     * 停止框架
     * @return true 停止成功，false 停止失败
     */
    virtual bool Stop() override;

    /**
     * 关闭框架并释放核心资源
     */
    virtual void Close() override;

    /**
     * 判断框架是否处于运行态
     * @return true 运行中；false 非运行中
     */
    virtual bool IsRunning() const override;

    /**
     * 生成新的对象 GUID
     * @return 新分配的对象 GUID
     */
    virtual SFObjectGUID NewObjectGUID() override;

#pragma endregion SkywalkerFramework

private:
    /** 插件管理器 */
    SF_PTR(SFPluginManager)
    PluginManager{};

    /** 服务管理器 */
    SF_PTR(SSFFrameworkServiceManager)
    ServiceManager{};

    /** 当前运行状态 */
    ERunningState RunningState{ERunningState::RunningState_Create};

    /** 当前累计分配的对象 GUID（递增） */
    SFObjectGUID AddObjectGUID{SF_OBJECT_INVALID_GUID};
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
