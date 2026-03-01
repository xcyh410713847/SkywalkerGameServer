/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SSFServiceManager.h
**作者: shyfan
**日期: 2024/01/22 16:57:54
**功能: 服务管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__

#include "Include/SFCore.h"

#include "Core/Object/SSFObjectManager.h"
#include "Core/Service/FrameworkService/SSFFrameworkService.h"
#include "Core/Service/LevelService/SSFLevelService.h"

SF_NAMESPACE_BEGIN

class SSFFrameworkService;

SF_TEMPLATE_CLASS(SSFService, ServiceObject)
class SSFServiceManager : public SSFObjectManager<ServiceObject>
{
public:
    SSFServiceManager(SSFObjectContext &InContext, SFObjectErrors &InErrors)
        : SSFObjectManager<ServiceObject>(InContext, InErrors) {};
    virtual ~SSFServiceManager() {};

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
     * 释放
     */
    virtual void Release(SFObjectErrors &Errors) override
    {
        ServiceMap.clear();
        SSFObjectManager<ServiceObject>::Release(Errors);
    };

    /**
     * 获取Service
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
     * 获取Service
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
    SFMap<SFString, SFObjectGUID> ServiceMap;
};

typedef SSFServiceManager<SSFFrameworkService> SSFFrameworkServiceManager;
typedef SSFServiceManager<SSFLevelService> SSFLevelServiceManager;

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__
