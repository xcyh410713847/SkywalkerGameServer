/*************************************************************************
**文件: SkywalkerServerFramework\Core\Service\SSFServiceManager.h
**作者: shyfan
**日期: 2024/01/22 16:57:54
**功能: 服务管理器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObjectManager.h"
#include "Core/Service/FrameworkService/SSFFrameworkService.h"
#include "Core/Service/LevelService/SSFLevelService.h"

SSF_NAMESPACE_BEGIN

class SSFFrameworkService;

SSF_TEMPLATE_CLASS(SSFService, ServiceObject)
class SSFServiceManager : public SSFObjectManager<ServiceObject>
{
    SSF_OBJECT_CLASS(SSFServiceManager)

public:
    SSFServiceManager(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
        : SSFObjectManager<ServiceObject>(InContext, InErrors){};
    virtual ~SSFServiceManager(){};

    virtual bool Tick(SSFObjectErrors &Errors)
    {
        SSF_COMMON_ITERATOR(IterService, ServiceMap)
        {
            auto IterObject = SSFObjectManager<ServiceObject>::FindObject(IterService->second);
            if (SSF_PTR_INVALID(IterObject))
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
    virtual void Release(SSFObjectErrors &Errors) override
    {
        ServiceMap.clear();
        SSFObjectManager<ServiceObject>::Release(Errors);
    };

    /**
     * 获取Service
     */
    inline SSF_PTR(SSFService) GetService(const SSFString &ServiceName)
    {
        auto Iter = ServiceMap.find(ServiceName);
        if (Iter == ServiceMap.end())
        {
            return nullptr;
        }

        auto IterObject = SSFObjectManager<ServiceObject>::FindObject(Iter->second);
        if (SSF_PTR_INVALID(IterObject))
        {
            return nullptr;
        }

        return SSF_PTR_DYNAMIC_CAST(SSFService)(IterObject);
    };

    /**
     * 获取Service
     */
    template <typename ServiceT>
    SSF_PTR(ServiceT)
    GetService()
    {
        SSF_ASSERT_IS_BASE_OF(ServiceObject, ServiceT);

        SSFString ServiceName{};
        SSF_CLASS_NAME(ServiceT, ServiceName);

        if (ServiceMap.find(ServiceName) == ServiceMap.end())
        {
            // 新建一个
            SSFServiceContext Context{};
            SSFObjectErrors Errors{};
            SSF_PTR(ServiceT)
            NewService = SSFObject::NewObject<ServiceT>(Context, Errors);
            if (SSF_PTR_INVALID(NewService))
            {
                return nullptr;
            }

            SSFObjectManager<ServiceObject>::AddObject(Errors, NewService);
            ServiceMap.insert(std::make_pair(ServiceName, NewService->GetObjectGUID()));

            NewService->Init(Errors);
            NewService->Start(Errors);
        }

        return SSF_PTR_DYNAMIC_CAST(ServiceT)(GetService(ServiceName));
    };

protected:
    SSFMap<SSFString, SSFObjectGUID> ServiceMap;
};

typedef SSFServiceManager<SSFFrameworkService> SSFFrameworkServiceManager;
typedef SSFServiceManager<SSFLevelService> SSFLevelServiceManager;

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFServiceManager_H__
