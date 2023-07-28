/*************************************************************************
**文件: SSFModule.h
**作者: shyfan
**日期: 2023/07/28 10:55:15
**功能: 模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__

#include "SSFIModule.h"

namespace Skywalker
{
    namespace ServerFramework
    {
        class SSFCModule : public SSFIModule
        {
        public:
            SSFCModule();
            virtual ~SSFCModule();
        };
    }
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
