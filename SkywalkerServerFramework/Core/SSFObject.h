/*************************************************************************
**文件: SkywalkerServerFramework\Core\SSFObject.h
**作者: shyfan
**日期: 2023/07/30 12:57:02
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
