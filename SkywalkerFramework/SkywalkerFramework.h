/*************************************************************************
**文件: SkywalkerFramework\SkywalkerFramework.h
**作者: shyfan
**日期: 2023/08/08 10:07:38
**功能: SkywalkerFramework
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_H__
#define __SKYWALKER_SERVER_FRAMEWORK_H__

#include "Include/SSFCore.h"
#include "Include/SFramework.h"

#include "Core/Object/SSFObject.h"
#include "Core/Service/SSFServiceManager.h"

SSF_NAMESPACE_BEGIN

/**
 * Skywalker Framework
 */
class CSkywalkerFramework : public SkywalkerFramework, public SSFObject
{
    typedef ESkywalkerServerFrameworkRunningState ERunningState;

public:
    CSkywalkerFramework() {};
    virtual ~CSkywalkerFramework() {};

    /**
     * 获取 Service
     */
    template <typename ServiceT>
    SSF_PTR(ServiceT)
    GetService() const
    {
        return ServiceManager->GetService<ServiceT>();
    }

#pragma region SkywalkerFramework

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
     * new一个ObjectGUID
     */
    virtual SSFObjectGUID NewObjectGUID() override;

    /**
     * 获取插件管理器
     */
    virtual SSF_PTR(SSFPluginManager) GetPluginManager() const override
    {
        return PluginManager;
    }

#pragma endregion SkywalkerFramework

private:
    SSF_PTR(SSFPluginManager)
    PluginManager{};

    SSF_PTR(SSFFrameworkServiceManager)
    ServiceManager{};

    ERunningState RunningState{ERunningState::SkywalkerServerFrameworkRunningState_Create};

    SSFObjectGUID AddObjectGUID{SSF_OBJECT_INVALID_GUID};
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_H__
