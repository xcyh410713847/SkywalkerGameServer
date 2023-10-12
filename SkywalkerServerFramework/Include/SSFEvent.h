/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFEvent.h
**作者: shyfan
**日期: 2023/10/09 15:11:47
**功能: 事件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__

#include "SSFCore.h"

SSF_NAMESPACE_BEGIN

#pragma region 事件类型

struct SSFEventType
{
    SSFEventType()
    {
        memset(this, 0, sizeof(SSFEventType));
    }

    SSFEventType(int InMainType, int InSubType)
    {
        this->MainType = InMainType;
        this->SubType = InSubType;
    }

    void SetType(int InMainType, int InSubType)
    {
        this->MainType = InMainType;
        this->SubType = InSubType;
    }

private:
    int MainType = 0;
    int SubType = 0;
};

// 事件主类型
enum SSFEventMainType
{
    SSFEventType_Invalid = 0,     // 无效事件
    SSFEventType_SystemMax = 999, // 系统事件最大值
};

#pragma endregion 事件类型

#pragma region 事件注册

#pragma endregion 事件注册

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFEvent_H__
