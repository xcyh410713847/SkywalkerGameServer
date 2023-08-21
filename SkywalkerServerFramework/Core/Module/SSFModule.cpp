/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModule.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:19
**功能: 模块
*************************************************************************/

#include "SSFModule.h"

SKYWALKER_SF_NAMESPACE_USE

#pragma region Object

void SSFOModule::Init(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Init
}

void SSFOModule::Awake(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Awake
}

void SSFOModule::Start(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Start
}

void SSFOModule::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    // TODO Shyfan 插件管理器Tick
}

void SSFOModule::Stop(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Stop
}

void SSFOModule::Sleep(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Sleep
}

void SSFOModule::Destroy(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Destroy
}

void SSFOModule::Release(SSFObjectErrors &Errors)
{
    delete this;
}

#pragma endregion Object

SSFOModule::SSFOModule()
{
}

SSFOModule::~SSFOModule()
{
}
