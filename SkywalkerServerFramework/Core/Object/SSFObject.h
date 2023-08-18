/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObject.h
**作者: shyfan
**日期: 2023/08/07 23:52:40
**功能: 对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__

#include "Include/SSFIObject.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCObject : public SSFIObject
{
#pragma region Object Base Interface

public:
    /**
     * 调用循序
     * Init -> Awake -> Start -> Tick -> Stop -> Sleep -> Destroy
     */

    /**
     * 初始化
     */
    virtual void Init(SSFObjectErrors &Errors) = 0;

    /**
     * 唤醒
     */
    virtual void Awake(SSFObjectErrors &Errors) = 0;

    /**
     * 开始
     */
    virtual void Start(SSFObjectErrors &Errors) = 0;

    /**
     * Tick
     */
    virtual void Tick(SSFObjectErrors &Errors, int DelayMS) = 0;

    /**
     * 结束
     */
    virtual void Stop(SSFObjectErrors &Errors) = 0;

    /**
     * 休眠
     */
    virtual void Sleep(SSFObjectErrors &Errors) = 0;

    /**
     * 销毁
     */
    virtual void Destroy(SSFObjectErrors &Errors) = 0;

    /**
     * 释放
     */
    virtual void Release(SSFObjectErrors &Errors) = 0;

#pragma endregion Object Base Interface

public:
    SSFCObject();
    virtual ~SSFCObject();
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
