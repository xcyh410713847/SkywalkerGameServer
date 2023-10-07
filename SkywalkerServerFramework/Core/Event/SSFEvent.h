/*************************************************************************
**文件: SkywalkerServerFramework\Core\Event\SSFEvent.h
**作者: shyfan
**日期: 2023/10/07 20:50:03
**功能: 事件
*************************************************************************/

#include "Include/SSFCore.h"

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__

#include "Core/Object/SSFObject.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFEvent : public SSFObject
{
    SSF_OBJECT_CLASS(SSFEvent)
};

SKYWALKER_SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__
