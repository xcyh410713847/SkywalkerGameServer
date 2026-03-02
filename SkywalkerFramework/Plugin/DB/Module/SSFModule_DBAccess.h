/*************************************************************************
**文件: SkywalkerFramework\Plugin\DB\Module\SSFModule_DBAccess.h
**作者: shyfan
**日期: 2026/03/02
**功能: 数据访问模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_DBACCESS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_DBACCESS_H__

#include "Core/Module/SSFModule.h"

SF_NAMESPACE_BEGIN

class SSFModule_DBAccess : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFModule_DBAccess(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SSFModule_DBAccess() {};

    bool IsReady() const;

private:
    SFBool bReady = FALSE;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_DBACCESS_H__
