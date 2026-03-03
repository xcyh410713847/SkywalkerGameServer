/*************************************************************************
**文件: SkywalkerFramework\Plugin\DB\Module\SFModule_PlayerRepository.h
**作者: shyfan
**日期: 2026/03/02
**功能: 玩家数据仓储模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_PLAYERREPOSITORY_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_PLAYERREPOSITORY_H__

#include "Core/Module/SFModule.h"

SF_NAMESPACE_BEGIN

class SFModule_PlayerRepository : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_PlayerRepository(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_PlayerRepository() {};

    bool LoadPlayer(SFUInt64 PlayerId);
    bool SavePlayer(SFUInt64 PlayerId);
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_PLAYERREPOSITORY_H__
