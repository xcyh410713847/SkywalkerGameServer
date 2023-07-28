/*************************************************************************
**文件: Object.h
**作者: shyfan
**日期: 2023/07/28 10:39:09
**功能: 对象
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_OBJECT_H__
#define __SKYWALKER_FRAMEWORK_OBJECT_H__

#include "IObject.h"

namespace Skywalker
{
    namespace Framework
    {
        class CObject : public IObject
        {
        public:
            CObject();
            virtual ~CObject();
        };
    }
}

#endif // __SKYWALKER_FRAMEWORK_OBJECT_H__
