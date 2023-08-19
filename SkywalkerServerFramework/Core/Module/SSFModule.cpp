/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModule.cpp
**作者: shyfan
**日期: 2023/08/07 23:52:19
**功能: 模块
*************************************************************************/

#include "SSFModule.h"

SKYWALKER_SF_NAMESPACE_USE

#pragma region Object

void SSFCModule::Init(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Init
}

void SSFCModule::Awake(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Awake
}

void SSFCModule::Start(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Start
}

void SSFCModule::Tick(SSFObjectErrors &Errors, int DelayMS)
{
    // TODO Shyfan 插件管理器Tick
}

void SSFCModule::Stop(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Stop
}

void SSFCModule::Sleep(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Sleep
}

void SSFCModule::Destroy(SSFObjectErrors &Errors)
{
    // TODO Shyfan 插件管理器Destroy
}

void SSFCModule::Release(SSFObjectErrors &Errors)
{
    delete this;
}

#pragma endregion Object

SSFCModule::SSFCModule()
{
}

SSFCModule::~SSFCModule()
{
}
