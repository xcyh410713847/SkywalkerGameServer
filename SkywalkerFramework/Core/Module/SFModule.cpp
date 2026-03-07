/*************************************************************************
**文件: SkywalkerFramework\Core\Module\SFModule.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:19
**功能: 模块
*************************************************************************/

#include "SFModule.h"

#include "Include/SFILog.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule, ESFLogLevel::Debug);

#pragma region Object

/** 初始化阶段 */
void SSFModule::Init(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Init");
}

/** 唤醒阶段 */
void SSFModule::Awake(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Awake");
}

/** 启动阶段 */
void SSFModule::Start(SFObjectErrors &Errors)
{
    SF_LOG_DEBUG_MODULE("Start");
}

/** Tick 阶段 */
void SSFModule::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
}

/** 停止阶段 */
void SSFModule::Stop(SFObjectErrors &Errors)
{

    SF_LOG_DEBUG_MODULE("Stop");
}

/** 休眠阶段 */
void SSFModule::Sleep(SFObjectErrors &Errors)
{

    SF_LOG_DEBUG_MODULE("Sleep");
}

/** 销毁阶段 */
void SSFModule::Destroy(SFObjectErrors &Errors)
{

    SF_LOG_DEBUG_MODULE("Destroy");
}

#pragma endregion Object

/** 构造函数 */
SSFModule::SSFModule(SSFModuleContext &InContext, SFObjectErrors &InErrors)
    : SSFObject(InContext, InErrors)
{
    Plugin = InContext.Plugin;
}

/** 析构函数 */
SSFModule::~SSFModule()
{
}
