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

SSF_NAMESPACE_BEGIN

class SSFObjectSimple
{

#pragma region NewObject

private:
    static SSFMap<std::string, SKYWALKER_POOL_PTR(SSFObjectSimple)> ObjectPoolMap;

public:
    template <typename T, typename... Params>
    static SSF_PTR(T) NewObject(Params... param)
    {
        // 从对象池中获取对象
        std::string ClassName = SSF_CLASS_NAME(T);
        auto Iterator = SSFObjectSimple::ObjectPoolMap.find(ClassName);
        if (Iterator != SSFObjectSimple::ObjectPoolMap.end())
        {
            SKYWALKER_POOL_PTR(SSFObjectSimple)
            ObjectPool = Iterator->second;
            if (SSF_PTR_VALID(ObjectPool))
            {
                SSFObjectSimple *Object = ObjectPool->Get();
                if (SSF_PTR_VALID(Object))
                {
                    return (T *)Object;
                }
            }
        }

        return new T(param...);
    }

    template <typename T, typename... Params>
    static SSF_SHARED_PTR(T) NewSharedObject(Params... param)
    {
        return SSF_MAKE_SHARED_PTR(T, param...);
    }

    static bool RemoveObject(SSF_PTR_OBJECT_SIMPLE Object)
    {
        if (!SSF_PTR_VALID(Object))
        {
            return false;
        }

        SSFUInt PoolSize = Object->GetPoolSize();
        if (PoolSize <= 0)
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
            ObjectPool = SKYWALKER_POOL_NEW(SSFObjectSimple, PoolSize);

            SSFObjectSimple::ObjectPoolMap.insert(std::make_pair(ClassName, ObjectPool));

            Iterator = SSFObjectSimple::ObjectPoolMap.find(ClassName);
            if (Iterator == SSFObjectSimple::ObjectPoolMap.end())
            {
                return false;
            }
        }

        SKYWALKER_POOL_PTR(SSFObjectSimple)
        ObjectPool = Iterator->second;
        if (!SSF_PTR_VALID(ObjectPool))
        {
            return false;
        }

        ObjectPool->Recycle(Object);

        return true;
    }

#pragma endregion NewObject

#pragma region SSFObjectSimple

public:
    SSFObjectSimple();
    virtual ~SSFObjectSimple();

    /**
     * 释放
     */
    virtual void Release();

#pragma endregion SSFObjectSimple

#pragma region Object Interface

public:
    /**
     * 获取类名称
     * @return 类名称
     */
    virtual const std::string GetObjectClassName() = 0;

    /**
     * 获取对象池大小
     */
    virtual const SSFUInt &GetPoolSize() = 0;

#pragma endregion Object Interface
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFObjectSimple_H__
