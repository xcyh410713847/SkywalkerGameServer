/*************************************************************************
**文件: Module.h
**作者: shyfan
**日期: 2023/07/28 10:40:35
**功能: 模块
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_MODULE_H__
#define __SKYWALKER_FRAMEWORK_MODULE_H__

#include "IModule.h"

namespace Skywalker
{
    namespace Framework
    {
        class CModule : public IModule
        {
        public:
            CModule();
            virtual ~CModule();
        };
    }
}

#endif // __SKYWALKER_FRAMEWORK_MODULE_H__
