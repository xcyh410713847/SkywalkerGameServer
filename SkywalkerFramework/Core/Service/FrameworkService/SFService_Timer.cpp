/*************************************************************************
**文件: SkywalkerFramework\Core\Service\FrameworkService\SFService_Timer.cpp
**作者: shyfan
**日期: 2024/01/24 19:46:34
**功能: 时间 Service
*************************************************************************/

#include "SFService_Timer.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFService_Timer, ESFLogLevel::Debug);

/** 构造函数 */
SSFService_Timer::SSFService_Timer(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFFrameworkService(InContext, InErrors)
{
}

/** 析构函数 */
SSFService_Timer::~SSFService_Timer()
{
}

#pragma region Process

/** 初始化阶段：重置计时器 */
bool SSFService_Timer::Init(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Init(InErrors);

    SkywalkerSFTimer::Reset();

    return true;
}

/** 启动阶段：开始首帧计时 */
bool SSFService_Timer::Start(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Start(InErrors);

    SkywalkerSFTimer::Tick();

    return true;
}

/** Tick 阶段：推进计时器 */
bool SSFService_Timer::Tick(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Tick(InErrors);

    SkywalkerSFTimer::Tick();

    return true;
}

/** 停止阶段 */
bool SSFService_Timer::Stop(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Stop(InErrors);

    return true;
}

/** 销毁阶段 */
void SSFService_Timer::Destroy(SFObjectErrors &InErrors)
{
    SSFFrameworkService::Destroy(InErrors);
}

#pragma endregion Process
