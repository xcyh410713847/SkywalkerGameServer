/*************************************************************************
**文件: SkywalkerFramework\Core\Module\SFModule.h
**作者: shyfan
**日期: 2023/08/07 23:52:04
**功能: 模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__

#include "Include/SFCore.h"

#include "Core/Object/SFObject.h"

SF_NAMESPACE_BEGIN

class SFPlugin;

struct SSFModuleContext : public SSFObjectContext
{
    SF_PTR(SFPlugin)
    Plugin{};
};

using SFModuleContext = SSFModuleContext;

class SSFModule : public SSFObject
{
#pragma region Module

public:
    /**
     * 初始化
     */
    virtual void Init(SFObjectErrors &Errors);

    /**
     * 唤醒
     */
    virtual void Awake(SFObjectErrors &Errors);

    /**
     * 开始
     */
    virtual void Start(SFObjectErrors &Errors);

    /**
     * Tick
     */
    virtual void Tick(SFObjectErrors &Errors, SFUInt64 DelayMS);

    /**
     * 结束
     */
    virtual void Stop(SFObjectErrors &Errors);

    /**
     * 休眠
     */
    virtual void Sleep(SFObjectErrors &Errors);

    /**
     * 销毁
     */
    virtual void Destroy(SFObjectErrors &Errors);

#pragma endregion Module

public:
    SSFModule(SSFModuleContext &InContext, SFObjectErrors &InErrors);
    virtual ~SSFModule();

    /**
     * 获取插件
     */
    SF_PTR(SFPlugin)
    GetPlugin()
    {
        return Plugin;
    };

private:
    SF_PTR(SFPlugin)
    Plugin;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
