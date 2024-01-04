/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObject.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:30
**功能: 对象
*************************************************************************/

#include "SSFObject.h"

#include "Include/SSFILog.h"

#include "SkywalkerServerFramework.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFObject, LogLevel_Debug);

SSFObject::SSFObject()
{
}

SSFObject::SSFObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
{
    Context = InContext;
}

SSFObject::~SSFObject()
{
}

/**
 * 创建对象
 */
template <typename T>
SSF_SHARED_PTR(T)
SSFObject::NewSharedObject()
{
    return Context.SSFramework->NewSharedObject<T>();
}

/**
 * 创建对象
 */
template <typename T>
SSF_SHARED_PTR(T)
SSFObject::NewSharedObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
{
    return Context.SSFramework->NewSharedObject<T>(InContext, InErrors);
}

/**
 * 创建对象
 */
template <typename T>
SSF_PTR(T)
SSFObject::NewObject()
{
    return Context.SSFramework->NewObject<T>();
}

/**
 * 创建对象
 */
template <typename T>
SSF_PTR(T)
SSFObject::NewObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
{
    return Context.SSFramework->NewObject<T>(InContext, InErrors);
}

#pragma region Object

void SSFObject::Create(SSFObjectErrors &Errors, SSFObjectContext &Context) {}

void SSFObject::Init(SSFObjectErrors &Errors) {}

void SSFObject::Awake(SSFObjectErrors &Errors) {}

void SSFObject::Start(SSFObjectErrors &Errors) {}

void SSFObject::Tick(SSFObjectErrors &Errors, int DelayMS) {}

void SSFObject::Stop(SSFObjectErrors &Errors) {}

void SSFObject::Sleep(SSFObjectErrors &Errors) {}

void SSFObject::Destroy(SSFObjectErrors &Errors) {}

void SSFObject::Release(SSFObjectErrors &Errors)
{
    delete this;
}

#pragma endregion Object
