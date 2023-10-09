/*************************************************************************
**文件: SkywalkerServerFramework\Core\Event\SSFObject_Event.h
**作者: shyfan
**日期: 2023/10/09 20:33:06
**功能: 事件对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFObject_Event_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFObject_Event_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObjectSimple.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFObject_Event : public SSFObjectSimple
{
    SSF_OBJECT_CLASS(SSFObject_Event);

public:
    SSFObject_Event();
    virtual ~SSFObject_Event();
};

SKYWALKER_SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFObject_Event_H__
