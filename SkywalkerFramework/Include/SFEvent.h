/*************************************************************************
**文件: SkywalkerFramework\Include\SFEvent.h
**作者: shyfan
**日期: 2023/10/09 15:11:47
**功能: 事件
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_SFEVENT_H__
#define __SKYWALKER_FRAMEWORK_SFEVENT_H__

#include "SkywalkerEvent/SkywalkerEvent.h"

#include "SFCore.h"

SF_NAMESPACE_BEGIN

/**
 * 注意：
 *      无论是主类型还是子类型都不建议使用 All 事件
 */

// 事件主类型
enum ESFEventMainType
{
    All = 0,         // 全部事件，不建议使用
    Basic = 1,       // 框架事件
    Plugin = 2,      // 插件事件
    SystemMax = 999, // 系统事件最大值
};

#pragma region 插件事件子类型

class SFPlugin;

/**
 * All 事件
 */
#define SFEventSubType_Plugin_All 0
struct SSFEventPluginAll
{
    SF_PTR(SFPlugin)
    Plugin;
};

/**
 * Init 事件
 */
#define SFEventSubType_Plugin_Init 1

/**
 * Awake 事件
 */
#define SFEventSubType_Plugin_Awake 2

/**
 * Start 事件
 */
#define SFEventSubType_Plugin_Start 3

/**
 * Tick 事件
 */
#define SFEventSubType_Plugin_Tick 4

/**
 * Stop 事件
 */
#define SFEventSubType_Plugin_Stop 5

/**
 * Sleep 事件
 */
#define SFEventSubType_Plugin_Sleep 6

/**
 * Destroy 事件
 */
#define SFEventSubType_Plugin_Destroy 7

/**
 * Release 事件
 */
#define SFEventSubType_Plugin_Release 8

/**
 * Max 事件
 */
#define SFEventSubType_Plugin_Max 999

#pragma endregion 插件事件子类型

typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventMainID SFEventMainType;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventSubID SFEventSubType;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventID SFEventID;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventParam SFEventParam;
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventParamSize SFEventParamSize;

SF_NAMESPACE_END

#endif //__SKYWALKER_FRAMEWORK_SFEVENT_H__
