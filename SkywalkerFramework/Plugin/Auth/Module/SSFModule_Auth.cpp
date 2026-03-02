/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\Module\SSFModule_Auth.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 认证模块
*************************************************************************/

#include "SSFModule_Auth.h"

#include "Include/SFILog.h"

#include "Core/Service/SSFGameplayServiceGateway.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_Auth, ESFLogLevel::Debug);

void SSFModule_Auth::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);

    SF_LOG_FRAMEWORK("Auth module init");
}

void SSFModule_Auth::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    SSFGameplayServiceGateway::Instance().RegisterAuthValidator(
        [this](const SFString &Token)
        {
            return ValidateToken(Token);
        });

    SF_LOG_FRAMEWORK("Auth module start");
}

void SSFModule_Auth::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterAuthValidator(nullptr);

    SF_LOG_FRAMEWORK("Auth module stop");

    SSFModule::Stop(Errors);
}

void SSFModule_Auth::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("Auth module destroy");

    SSFModule::Destroy(Errors);
}

bool SSFModule_Auth::ValidateToken(const SFString &Token) const
{
    return !Token.empty();
}
