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

#include "Core/Object/SSFObject.h"
#include "Core/Service/SSFFrameworkService.h"
#include "Core/Service/SSFServiceManager.h"

SSF_NAMESPACE_BEGIN

/**
 * Skywalker Server Framework
 */
class CSkywalkerServerFramework : public SkywalkerServerFramework, public SSFObject
{
    typedef ESkywalkerServerFrameworkRunningState ERunningState;

    SSF_OBJECT_CLASS(CSkywalkerServerFramework);

public:
    CSkywalkerServerFramework(){};
    virtual ~CSkywalkerServerFramework(){};

    /**
     * 获取 Service
     */
    template <typename ServiceT>
    SSF_PTR(ServiceT)
    GetService() const
    {
        return ServiceManager->GetService<ServiceT>();
    }

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

#pragma endregion SkywalkerServerFramework

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
