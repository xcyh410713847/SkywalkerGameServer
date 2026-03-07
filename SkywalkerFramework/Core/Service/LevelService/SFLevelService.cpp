/*************************************************************************
**文件: SkywalkerFramework\Core\Service\SFLevelService.cpp
**作者: shyfan
**日期: 2024/01/22 12:28:39
**功能: SSFLevelService
*************************************************************************/

#include "SFLevelService.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFLevelService, ESFLogLevel::Debug);

/** 构造函数 */
SSFLevelService::SSFLevelService(SSFServiceContext &InContext, SFObjectErrors &InErrors)
    : SSFService(InContext, InErrors)
{
}

/** 析构函数 */
SSFLevelService::~SSFLevelService()
{
}

#pragma region Process

/** 初始化阶段 */
bool SSFLevelService::Init(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Init");

    return true;
}

/** 启动阶段 */
bool SSFLevelService::Start(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Start");

    return true;
}

/** Tick 阶段 */
bool SSFLevelService::Tick(SFObjectErrors &InErrors)
{
    return true;
}

/** 停止阶段 */
bool SSFLevelService::Stop(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Stop");

    return true;
}

/** 销毁阶段 */
void SSFLevelService::Destroy(SFObjectErrors &InErrors)
{
    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Process
