/*************************************************************************
**文件: SkywalkerServerFramework/Object/SSFObject.h
**作者: shyfan
**日期: 2023/07/28 10:57:12
**功能: 对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__

#include "Include/SSFIObject.h"

namespace Skywalker
{
    namespace ServerFramework
    {
        class SSFCObject : public SSFIObject
        {
        public:
            SSFCObject();
            virtual ~SSFCObject();
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
