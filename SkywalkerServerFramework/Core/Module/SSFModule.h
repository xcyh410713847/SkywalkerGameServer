/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModule.h
**作者: shyfan
**日期: 2023/07/30 12:32:57
**功能: 模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__

#include "Include/SSFIModule.h"

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
