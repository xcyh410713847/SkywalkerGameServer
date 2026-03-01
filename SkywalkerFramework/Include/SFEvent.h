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
 *      无论主类型还是子类型，都不建议使用 All 事件
 */

/**
 * 事件主类型
 */
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
 * 插件 All 子类型
 */
#define SFEventSubType_Plugin_All 0
struct SSFEventPluginAll
{
    /**
     * 触发事件的插件对象
     */
    SF_PTR(SFPlugin)
    Plugin;
};

/**
 * 插件 Init 子类型
 */
#define SFEventSubType_Plugin_Init 1

/**
 * 插件 Awake 子类型
 */
#define SFEventSubType_Plugin_Awake 2

/**
 * 插件 Start 子类型
 */
#define SFEventSubType_Plugin_Start 3

/**
 * 插件 Tick 子类型
 */
#define SFEventSubType_Plugin_Tick 4

/**
 * 插件 Stop 子类型
 */
#define SFEventSubType_Plugin_Stop 5

/**
 * 插件 Sleep 子类型
 */
#define SFEventSubType_Plugin_Sleep 6

/**
 * 插件 Destroy 子类型
 */
#define SFEventSubType_Plugin_Destroy 7

/**
 * 插件 Release 子类型
 */
#define SFEventSubType_Plugin_Release 8

/**
 * 插件子类型最大值
 */
#define SFEventSubType_Plugin_Max 999

#pragma endregion 插件事件子类型

/**
 * 事件主类型ID
 */
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventMainID SFEventMainType;

/**
 * 事件子类型ID
 */
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventSubID SFEventSubType;

/**
 * 事件唯一ID
 */
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventID SFEventID;

/**
 * 事件参数指针
 */
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventParam SFEventParam;

/**
 * 事件参数大小
 */
typedef SKYWALKER_EVENT_NAMESPACE::SkywalkerEventParamSize SFEventParamSize;

SF_NAMESPACE_END

#endif //__SKYWALKER_FRAMEWORK_SFEVENT_H__
