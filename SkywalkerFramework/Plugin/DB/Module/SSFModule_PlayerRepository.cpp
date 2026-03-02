/*************************************************************************
**文件: SkywalkerFramework\Plugin\DB\Module\SSFModule_PlayerRepository.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 玩家数据仓储模块
*************************************************************************/

#include "SSFModule_PlayerRepository.h"

#include "Include/SFILog.h"

#include "Core/Service/SSFGameplayServiceGateway.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_PlayerRepository, ESFLogLevel::Debug);

void SSFModule_PlayerRepository::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    SF_LOG_FRAMEWORK("PlayerRepository module init");
}

void SSFModule_PlayerRepository::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    SSFGameplayServiceGateway::Instance().RegisterPlayerLoader(
        [this](SFUInt64 PlayerId)
        {
            return LoadPlayer(PlayerId);
        });

    SF_LOG_FRAMEWORK("PlayerRepository module start");
}

void SSFModule_PlayerRepository::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterPlayerLoader(nullptr);

    SF_LOG_FRAMEWORK("PlayerRepository module stop");

    SSFModule::Stop(Errors);
}

void SSFModule_PlayerRepository::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("PlayerRepository module destroy");

    SSFModule::Destroy(Errors);
}

bool SSFModule_PlayerRepository::LoadPlayer(SFUInt64 PlayerId)
{
    SF_LOG_FRAMEWORK("LoadPlayer, PlayerId " << PlayerId);
    return true;
}

bool SSFModule_PlayerRepository::SavePlayer(SFUInt64 PlayerId)
{
    SF_LOG_FRAMEWORK("SavePlayer, PlayerId " << PlayerId);
    return true;
}
