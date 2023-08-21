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

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFOModule : public SSFObject
{
#pragma region Object

public:
    /**
     * 初始化
     */
    virtual void Init(SSFObjectErrors &Errors) override;

    /**
     * 唤醒
     */
    virtual void Awake(SSFObjectErrors &Errors) override;

    /**
     * 开始
     */
    virtual void Start(SSFObjectErrors &Errors) override;

    /**
     * Tick
     */
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS) override;

    /**
     * 结束
     */
    virtual void Stop(SSFObjectErrors &Errors) override;

    /**
     * 休眠
     */
    virtual void Sleep(SSFObjectErrors &Errors) override;

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &Errors) override;

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) override;

#pragma endregion Object

public:
    SSFOModule();
    virtual ~SSFOModule();

    /**
     * 获取模块名称
     * @return 模块名称
     */
    virtual const std::string GetName()
    {
        return SKYWALKER_SF_CLASS_NAME(SSFCModule);
    };
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_MODULE_H__
