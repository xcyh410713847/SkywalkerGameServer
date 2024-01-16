/*************************************************************************
**文件: SkywalkerServerFramework\Core\Module\SSFModule.h
**作者: shyfan
**日期: 2023/08/07 23:52:04
**功能: 模块
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"

SSF_NAMESPACE_BEGIN

class SSFOPluginManager;

struct SSFModuleContext : public SSFObjectContext
{
    SSF_PTR(SSFOPluginManager)
    PluginManager{};
};

class SSFOModule : public SSFObject
{
    SSF_OBJECT_CLASS(SSFOModule);

#pragma region Module

public:
    /**
     * 初始化
     */
    virtual void Init(SSFObjectErrors &Errors);

    /**
     * 唤醒
     */
    virtual void Awake(SSFObjectErrors &Errors);

    /**
     * 开始
     */
    virtual void Start(SSFObjectErrors &Errors);

    /**
     * Tick
     */
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS);

    /**
     * 结束
     */
    virtual void Stop(SSFObjectErrors &Errors);

    /**
     * 休眠
     */
    virtual void Sleep(SSFObjectErrors &Errors);

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &Errors);

#pragma endregion Module

public:
    SSFOModule(SSFModuleContext &InContext, SSFObjectErrors &InErrors);
    SSFOModule(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
        : SSFObject(InContext, InErrors){};
    virtual ~SSFOModule();

    /**
     * 获取插件管理器
     */
    SSF_PTR(SSFOPluginManager)
    GetPluginManager()
    {
        return PluginManager;
    };

private:
    SSF_PTR(SSFOPluginManager)
    PluginManager;
};

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
