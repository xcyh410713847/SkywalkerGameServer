/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFEvent.h
**作者: shyfan
**日期: 2023/10/09 15:11:47
**功能: 事件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__

#include "Include/SSFCore.h"
#include "SkywalkerEvent/SkywalkerEvent.h"

#include "SSFCore.h"

SSF_NAMESPACE_BEGIN

/**
 * 注意：
 *      无论是主类型还是子类型都不建议使用 All 事件
 */

// 事件主类型
enum SSFEEventMainType
{
    SSFEventMainType_all = 0,         // 全部事件，不建议使用
    SSFEventMainType_Framework = 1,   // 框架事件
    SSFEventMainType_Plugin = 2,      // 插件事件
    SSFEventMainType_SystemMax = 999, // 系统事件最大值
};

#pragma region 插件事件子类型

class SSFOPlugin;

/**
 * All 事件
 */
#define SSFEventSubType_Plugin_All 0
struct SSFEventPluginAll
{
    SSF_PTR(SSFOPlugin)
    Plugin;
};

/**
 * Init 事件
 */
#define SSFEventSubType_Plugin_Init 1

/**
 * Awake 事件
 */
#define SSFEventSubType_Plugin_Awake 2

/**
 * Start 事件
 */
#define SSFEventSubType_Plugin_Start 3

/**
 * Tick 事件
 */
#define SSFEventSubType_Plugin_Tick 4

/**
 * Stop 事件
 */
#define SSFEventSubType_Plugin_Stop 5

/**
 * Sleep 事件
 */
#define SSFEventSubType_Plugin_Sleep 6

/**
 * Destroy 事件
 */
#define SSFEventSubType_Plugin_Destroy 7

/**
 * Release 事件
 */
#define SSFEventSubType_Plugin_Release 8

/**
 * Max 事件
 */
#define SSFEventSubType_Plugin_Max 999

#pragma endregion 插件事件子类型

typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventMainID SSFEventMainType;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventSubID SSFEventSubType;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventID SSFEventID;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventParam SSFEventParam;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventParamSize SSFEventParamSize;

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__
