﻿/*************************************************************************
**文件: SkywalkerServerFramework\Core\Event\SSFObject_Event.cpp
**作者: shyfan
**日期: 2023/10/09 20:33:32
**功能: 事件对象
*************************************************************************/

#include "SSFObject_Event.h"

#include "Include/SSFCore.h"
#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFObject_Event, LogLevel_Debug);

SSFObject_Event::SSFObject_Event()
    : SSFObjectSimple()
{
    EnablePool = true;
}

SSFObject_Event::~SSFObject_Event()
{
}

SKYWALKER_SF_PTR(SSFObject_Event)
SSFObject_Event::SetEventType(SSFEventType InEventType)
{
    this->EventType = InEventType;

    return this;
}

SKYWALKER_SF_PTR(SSFObject_Event)
SSFObject_Event::SetEventType(int InMainType, int InSubType)
{
    this->EventType.SetType(InMainType, InSubType);

    return this;
}

SKYWALKER_SF_PTR(SSFObject_Event)
SSFObject_Event::SetEventSkin(SSF_PTR_EVENT_SKIN InEventSkin)
{
    this->EventSkin = InEventSkin;

    return this;
}
