/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFCreator.h
**作者: shyfan
**日期: 2023/08/21 15:57:01
**功能: 创建者
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_CREATOR_H__
#define __SKYWALKER_SERVER_FRAMEWORK_CREATOR_H__

#include "SkywalkerDerived/SkywalkerDerived.h"

#include "SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

/**
 * 创建对象上下文
 */
struct SSFObjectCreatorContext
{
};

template <typename T>
SKYWALKER_SF_PTR(T)
NewObject(SSFObjectCreatorContext &Context)
{
    if (!SKYWALKER_IS_DERIVED(T, SSFObject))
    {
        return nullptr;
    }
    return new T(Context);
}

template <typename T, typename M>
SKYWALKER_SF_PTR(T)
NewObject(SSFObjectCreatorContext &Context, SKYWALKER_SF_PTR(M) Manager)
{
    if (!SKYWALKER_IS_DERIVED(T, SSFObject))
    {
        return nullptr;
    }
    return new T(Context, Manager);
}

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_CREATOR_H__
