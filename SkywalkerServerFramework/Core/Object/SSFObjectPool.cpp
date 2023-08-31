/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObjectPool.cpp
**作者: shyfan
**日期: 2023/08/30 16:25:14
**功能: 对象池
*************************************************************************/

#include "SSFObjectPool.h"
#include "Include/SSFErrors.h"

SKYWALKER_SF_NAMESPACE_USE

SSFObjectPool::SSFObjectPool()
    : SSFObject()
{
}

SSFObjectPool::~SSFObjectPool()
{
}

SSFObject *SSFObjectPool::GetObject()
{
    if (ObjectVector.empty())
    {
        return nullptr;
    }

    SSFObject *Object = ObjectVector.front();
    ObjectVector.pop_front();
    return Object;
}

void SSFObjectPool::RecycleObject(SKYWALKER_SF_PTR_OBJECT Object)
{
    if (!SKYWALKER_SF_PTR_VALID(Object))
    {
        return;
    }

    ObjectVector.push_back(Object);
}

void SSFObjectPool::Clear()
{
    SSFObjectErrors Errors;
    SKYWALKER_SF_COMMON_ITERATOR(Iterator, ObjectVector)
    {
        SSFObject *Object = *Iterator;
        if (SKYWALKER_SF_PTR_VALID(Object))
        {
            Object->Release(Errors);
        }
    }

    ObjectVector.clear();
}
