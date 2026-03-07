/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFFrameworkService.cpp
**作者: shyfan
**日期: 2024/01/22 12:29:48
**功能: SSFFrameworkService
*************************************************************************/

#include "SFFrameworkService.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFFrameworkService, ESFLogLevel::Debug);

/** 构造函数 */
SSFFrameworkService::SSFFrameworkService(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFService(InContext, InErrors)
{
}

/** 析构函数 */
SSFFrameworkService::~SSFFrameworkService()
{
}

#pragma region Process

/** 初始化阶段 */
bool SSFFrameworkService::Init(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Init");

    return true;
}

/** 启动阶段 */
bool SSFFrameworkService::Start(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Start");

    return true;
}

/** Tick 阶段 */
bool SSFFrameworkService::Tick(SFObjectErrors &InErrors)
{
    return true;
}

/** 停止阶段 */
bool SSFFrameworkService::Stop(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Stop");

    return true;
}

/** 销毁阶段 */
void SSFFrameworkService::Destroy(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Process
