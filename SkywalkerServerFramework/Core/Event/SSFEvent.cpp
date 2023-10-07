/*************************************************************************
**文件: SkywalkerServerFramework\Core\Event\SSFEvent.cpp
**作者: shyfan
**日期: 2023/10/07 20:51:00
**功能: 事件
*************************************************************************/

#include "SSFEvent.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFEvent, LogLevel_Debug);

#define SKYWALKER_SF_LOG_DEBUG_MODULE(content) SKYWALKER_SF_LOG_DEBUG(GetObjectClassName() << " " << content)
