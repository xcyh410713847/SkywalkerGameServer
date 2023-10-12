/*************************************************************************
**文件: SkywalkerServerFramework\Core\Event\SSFEventManager.cpp
**作者: shyfan
**日期: 2023/10/07 20:53:25
**功能: 事件管理器
*************************************************************************/

#include "SSFEventManager.h"

#include "Include/SSFILog.h"

SSF_NAMESPACE_USING

SSF_LOG_DEFINE(SSFOEventManager, LogLevel_Debug);

#define SSF_LOG_DEBUG_MODULE(content) SSF_LOG_DEBUG(GetObjectClassName() << " " << content)
