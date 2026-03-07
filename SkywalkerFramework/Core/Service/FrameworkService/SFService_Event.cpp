/*************************************************************************
**文件: SkywalkerFramework\Core\Service\FrameworkService\SFService_Event.cpp
**作者: shyfan
**日期: 2024/01/22 16:06:00
**功能: SSFService_Event
*************************************************************************/

#include "SFService_Event.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFService_Event, ESFLogLevel::Debug);

/** 构造函数 */
SSFService_Event::SSFService_Event(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFFrameworkService(InContext, InErrors)
{
}

/** 析构函数 */
SSFService_Event::~SSFService_Event()
{
}

#pragma region Process

/** 初始化阶段 */
bool SSFService_Event::Init(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Init(InErrors);

    return true;
}

/** 启动阶段 */
bool SSFService_Event::Start(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Start(InErrors);

    return true;
}

/** Tick 阶段 */
bool SSFService_Event::Tick(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Tick(InErrors);

    return true;
}

/** 停止阶段 */
bool SSFService_Event::Stop(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Stop(InErrors);

    return true;
}

/** 销毁阶段 */
void SSFService_Event::Destroy(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Destroy(InErrors);
}

#pragma endregion Process
