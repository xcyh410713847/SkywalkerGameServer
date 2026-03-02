/*************************************************************************
**文件: SkywalkerFramework\Plugin\Auth\Module\SSFModule_Auth.h
**作者: shyfan
**日期: 2026/03/02
**功能: 认证模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AUTH_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AUTH_H__

#include "Core/Module/SSFModule.h"

SF_NAMESPACE_BEGIN

class SSFModule_Auth : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_Auth(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_Auth() {};

    bool ValidateToken(const SFString &Token) const;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_AUTH_H__
