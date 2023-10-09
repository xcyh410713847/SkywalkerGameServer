/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObjectSimple.h
**作者: shyfan
**日期: 2023/10/09 20:07:23
**功能: 简单对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFObjectSimple_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFObjectSimple_H__

#include "SkywalkerPool/SkywalkerPool.h"

#include "Include/SSFCore.h"

#include "Core/Map/SSFMap.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFObjectSimple
{

#pragma region NewObject

private:
    static SSFMap<std::string, SKYWALKER_POOL_PTR(SSFObjectSimple)> ObjectPoolMap;

public:
    template <typename T, typename... Params>
    static SKYWALKER_SF_PTR(T) NewObject(Params... param)
    {
        // 从对象池中获取对象
        std::string ClassName = SKYWALKER_SF_CLASS_NAME(T);
        auto Iterator = SSFObjectSimple::ObjectPoolMap.find(ClassName);
        if (Iterator != SSFObjectSimple::ObjectPoolMap.end())
        {
            SKYWALKER_POOL_PTR(SSFObjectSimple)
            ObjectPool = Iterator->second;
            if (SKYWALKER_SF_PTR_VALID(ObjectPool))
            {
                SSFObjectSimple *Object = ObjectPool->Get();
                if (SKYWALKER_SF_PTR_VALID(Object))
                {
                    return (T *)Object;
                }
            }
        }

        return new T(param...);
    }

    static bool RemoveObject(SKYWALKER_SF_PTR_OBJECT_SIMPLE Object)
    {
        if (!SKYWALKER_SF_PTR_VALID(Object))
        {
            return false;
        }

        if (!Object->IsEnablePool())
        {
            delete Object;
            return true;
        }

        // 回收对象
        std::string ClassName = Object->GetObjectClassName();
        auto Iterator = SSFObjectSimple::ObjectPoolMap.find(ClassName);
        if (Iterator == SSFObjectSimple::ObjectPoolMap.end())
        {
            // 创建对象池
            SKYWALKER_POOL_PTR(SSFObjectSimple)
            ObjectPool = SKYWALKER_POOL_NEW(SSFObjectSimple, 100);

            SSFObjectSimple::ObjectPoolMap.insert(std::make_pair(ClassName, ObjectPool));

            Iterator = SSFObjectSimple::ObjectPoolMap.find(ClassName);
            if (Iterator == SSFObjectSimple::ObjectPoolMap.end())
            {
                return false;
            }
        }

        SKYWALKER_POOL_PTR(SSFObjectSimple)
        ObjectPool = Iterator->second;
        if (!SKYWALKER_SF_PTR_VALID(ObjectPool))
        {
            return false;
        }

        ObjectPool->Recycle(Object);

        return true;
    }

#pragma endregion NewObject

public:
    SSFObjectSimple();
    virtual ~SSFObjectSimple();

    /**
     * 获取类名称
     * @return 类名称
     */
    virtual const std::string GetObjectClassName() = 0;

    /**
     * 是否启用对象池
     */
    virtual bool IsEnablePool() = 0;
};

SKYWALKER_SF_NAMESPACE_END

#define SSF_OBJECT_CLASS(Class)                             \
public:                                                     \
    virtual const std::string GetObjectClassName() override \
    {                                                       \
        return SKYWALKER_SF_CLASS_NAME(Class);              \
    };                                                      \
                                                            \
    bool IsEnablePool() override                            \
    {                                                       \
        return EnablePool;                                  \
    };                                                      \
                                                            \
protected:                                                  \
    bool EnablePool = false;

/**
 * 创建对象
 */
#define SSF_NEW_OBJECT(T, ...) SKYWALKER_SF_NAMESPACE::SSFObjectSimple::NewObject<T>(__VA_ARGS__)

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFObjectSimple_H__
