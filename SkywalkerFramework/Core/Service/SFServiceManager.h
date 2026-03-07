/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFServiceManager.h
**作者: shyfan
**日期: 2024/01/22 16:57:54
**功能: 服务管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__

#include "Include/SFCore.h"

#include "Core/Object/SFObjectManager.h"
#include "Core/Service/FrameworkService/SFFrameworkService.h"
#include "Core/Service/LevelService/SFLevelService.h"

SF_NAMESPACE_BEGIN

class SSFFrameworkService;

SF_TEMPLATE_CLASS(SSFService, ServiceObject)
/**
 * 服务管理器（模板）
 *
 * 职责：
 * 1. 管理 ServiceObject 的创建、索引与获取
 * 2. 在 Tick 阶段驱动所有已注册服务
 * 3. 统一管理服务生命周期入口（懒加载时 Init + Start）
 */
class SSFServiceManager : public SSFObjectManager<ServiceObject>
{
public:
    /** 构造函数 */
    SSFServiceManager(SSFObjectContext &InContext, SFObjectErrors &InErrors)
        : SSFObjectManager<ServiceObject>(InContext, InErrors) {};
    /** 析构函数 */
    virtual ~SSFServiceManager() {};

    /**
     * 驱动所有服务 Tick
     * @return true 始终返回 true（当前实现）
     */
    virtual bool Tick(SFObjectErrors &Errors)
    {
        SF_COMMON_ITERATOR(IterService, ServiceMap)
        {
            auto IterObject = SSFObjectManager<ServiceObject>::FindObject(IterService->second);
            if (SF_PTR_INVALID(IterObject))
            {
                continue;
            }

            IterObject->Tick(Errors);
        }

        return true;
    }

    /**
     * 释放服务管理器
     * 说明：先清理服务索引，再释放基类对象。
     */
    virtual void Release(SFObjectErrors &Errors) override
    {
        ServiceMap.clear();
        SSFObjectManager<ServiceObject>::Release(Errors);
    };

    /**
     * 按名称获取服务
     * @param ServiceName 服务类名
     * @return 服务指针；未找到返回 nullptr
     */
    inline SF_PTR(SSFService) GetService(const SFString &ServiceName)
    {
        auto Iter = ServiceMap.find(ServiceName);
        if (Iter == ServiceMap.end())
        {
            return nullptr;
        }

        auto IterObject = SSFObjectManager<ServiceObject>::FindObject(Iter->second);
        if (SF_PTR_INVALID(IterObject))
        {
            return nullptr;
        }

        return SF_PTR_DYNAMIC_CAST(SSFService)(IterObject);
    };

    /**
     * 按类型获取服务（懒加载）
     * @tparam ServiceT 服务类型，必须继承自 ServiceObject
     * @return 服务指针；创建或转换失败返回 nullptr
     *
     * 说明：
     * 1. 若服务不存在，则自动 NewObject 创建
     * 2. 创建后自动 AddObject，并调用 Init 与 Start
     * 3. 之后返回缓存服务实例
     */
    template <typename ServiceT>
    SF_PTR(ServiceT)
    GetService()
    {
        SF_ASSERT_IS_BASE_OF(ServiceObject, ServiceT);

        SFString ServiceName{};
        SF_CLASS_NAME(ServiceT, ServiceName);

        if (ServiceMap.find(ServiceName) == ServiceMap.end())
        {
            // 新建一个
            SSFServiceContext Context{};
            SFObjectErrors Errors{};
            SF_PTR(ServiceT)
            NewService = SSFObject::NewObject<ServiceT>(Context, Errors);
            if (SF_PTR_INVALID(NewService))
            {
                return nullptr;
            }

            SSFObjectManager<ServiceObject>::AddObject(Errors, NewService);
            ServiceMap.insert(std::make_pair(ServiceName, NewService->GetObjectGUID()));

            NewService->Init(Errors);
            NewService->Start(Errors);
        }

        return SF_PTR_DYNAMIC_CAST(ServiceT)(GetService(ServiceName));
    };

protected:
    /** 服务索引表（Key: 服务类名，Value: 对象 GUID） */
    SFMap<SFString, SFObjectGUID> ServiceMap;
};

/** 框架级服务管理器类型别名 */
typedef SSFServiceManager<SSFFrameworkService> SSFFrameworkServiceManager;
/** 关卡级服务管理器类型别名 */
typedef SSFServiceManager<SSFLevelService> SSFLevelServiceManager;

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__
