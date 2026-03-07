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

/**
 * 模块创建上下文
 * 说明：用于注入所属插件指针。
 */
struct SSFModuleContext : public SSFObjectContext
{
    /** 所属插件 */
    SF_PTR(SFPlugin)
    Plugin{};
};

using SFModuleContext = SSFModuleContext;

/**
 * 模块基类
 * 说明：插件内部最小可调度单元，生命周期由插件统一驱动。
 */
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
     * 每帧更新
     * @param DelayMS 帧间隔（毫秒）
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
    /** 构造函数 */
    SSFModule(SSFModuleContext &InContext, SFObjectErrors &InErrors);
    /** 析构函数 */
    virtual ~SSFModule();

    /**
     * 获取插件
     * @return 所属插件指针
     */
    SF_PTR(SFPlugin)
    GetPlugin()
    {
        return Plugin;
    };

private:
    /** 所属插件 */
    SF_PTR(SFPlugin)
    Plugin;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
