/*************************************************************************
**文件: SkywalkerServerFramework\Core\Event\SSFEventManager.cpp
**作者: shyfan
**日期: 2023/10/07 20:53:25
**功能: 事件管理器
*************************************************************************/

#include "SSFEventManager.h"

#include "Include/SSFILog.h"

SKYWALKER_SF_NAMESPACE_USING

SKYWALKER_SF_LOG_DEFINE(SSFEventManager, LogLevel_Debug);

#define SKYWALKER_SF_LOG_DEBUG_MODULE(content) SKYWALKER_SF_LOG_DEBUG(GetObjectClassName() << " " << content)
