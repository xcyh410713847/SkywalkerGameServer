/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObject.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:30
**功能: 对象
*************************************************************************/

#include "SSFObject.h"

SKYWALKER_SF_NAMESPACE_USE

SSFObject::SSFObject()
{
}

SSFObject::~SSFObject()
{
}

void SSFObject::Create(SSFObjectErrors &Errors, SSFObjectCreatorContext &Context) {}

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
