/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFInterface.h
**作者: shyfan
**日期: 2023/08/15 11:47:54
**功能: 服务器框架接口
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_INTERFACE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_INTERFACE_H__

#include "SSFCore.h"
#include "SSFErrors.h"

SKYWALKER_SF_NAMESPACE_BEGIN

struct SSFInterface
{
    /**
     * 调用循序
     * Init -> Awake -> Start -> Tick -> Stop -> Sleep -> Destroy
     */

    /**
     * 初始化
     */
    virtual void Init(SSFInterfaceErrors &Errors) = 0;

    /**
     * 唤醒
     */
    virtual void Awake(SSFInterfaceErrors &Errors) = 0;

    /**
     * 开始
     */
    virtual void Start(SSFInterfaceErrors &Errors) = 0;

    /**
     * Tick
     */
    virtual void Tick(SSFInterfaceErrors &Errors, int DelayMS) = 0;

    /**
     * 结束
     */
    virtual void Stop(SSFInterfaceErrors &Errors) = 0;

    /**
     * 休眠
     */
    virtual void Sleep(SSFInterfaceErrors &Errors) = 0;

    /**
     * 销毁
     */
    virtual void Destroy(SSFInterfaceErrors &Errors) = 0;

    /**
     * 释放
     */
    virtual void Release(SSFInterfaceErrors &Errors) = 0;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_INTERFACE_H__
