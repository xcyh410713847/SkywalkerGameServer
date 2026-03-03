/*************************************************************************
**文件: SkywalkerFramework\Plugin\Level\Module\SFModule_WorldManager.h
**作者: shyfan
**日期: 2026/03/02
**功能: 世界管理模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_WORLDMANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_WORLDMANAGER_H__

#include "Core/Module/SFModule.h"

#include <set>

SF_NAMESPACE_BEGIN

class SFModule_WorldManager : public SSFModule
{
#pragma region Object

public:
    virtual void Init(SFObjectErrors &Errors) override;
    virtual void Start(SFObjectErrors &Errors) override;
    virtual void Stop(SFObjectErrors &Errors) override;
    virtual void Destroy(SFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SFModule_WorldManager(SFModuleContext &InContext, SFObjectErrors &InErrors)
        : SSFModule(InContext, InErrors)
    {
    }
    virtual ~SFModule_WorldManager() {};

    bool CreateWorld(SFUInt64 WorldId);
    bool EnterWorld(SFUInt64 PlayerId, SFUInt64 WorldId);
    bool LeaveWorld(SFUInt64 PlayerId, SFUInt64 WorldId);
    SFUInt64 GetWorldPlayerCount(SFUInt64 WorldId) const;

private:
    SFMap<SFUInt64, std::set<SFUInt64>> WorldPlayers;
    SFMap<SFUInt64, SFUInt64> PlayerWorldMap;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SSFMODULE_WORLDMANAGER_H__
