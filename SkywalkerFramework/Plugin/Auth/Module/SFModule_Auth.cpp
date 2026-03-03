/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\Module\SFModule_Auth.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 认证模块
*************************************************************************/

#include "SFModule_Auth.h"

#include "Include/SFILog.h"

#include "Core/Service/SFGameplayServiceGateway.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_Auth, ESFLogLevel::Debug);

void SFModule_Auth::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    SF_LOG_FRAMEWORK("Auth module init");
}

void SFModule_Auth::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    SSFGameplayServiceGateway::Instance().RegisterAuthValidator(
        [this](const SFString &Token)
        {
            return ValidateToken(Token);
        });

    SF_LOG_FRAMEWORK("Auth module start");
}

void SFModule_Auth::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterAuthValidator(nullptr);

    SF_LOG_FRAMEWORK("Auth module stop");

    SSFModule::Stop(Errors);
}

void SFModule_Auth::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("Auth module destroy");

    SSFModule::Destroy(Errors);
}

bool SFModule_Auth::ValidateToken(const SFString &Token) const
{
    return !Token.empty();
}
