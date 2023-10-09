﻿/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObject.h
**作者: shyfan
**日期: 2023/08/07 23:52:40
**功能: 对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__

#include "Include/SSFCore.h"
#include "Include/SSFErrors.h"

#include "SSFObjectSimple.h"

SKYWALKER_SF_NAMESPACE_BEGIN

/**
 * 对象创建上下文
 */
struct SSFObjectCreatorContext
{
	SSFObjectCreatorContext()
	{
		memset(this, 0, sizeof(SSFObjectCreatorContext));
	}
};

static const SSFObjectCreatorContext SSFObjectCreatorContextDefault;

class SSFObject : public SSFObjectSimple
{
public:
	/**
	 * 调用循序
	 * Create -> Init -> Awake -> Start -> Tick -> Stop -> Sleep -> Destroy -> Release
	 */

	virtual void Create(SSFObjectErrors &Errors, SSFObjectCreatorContext &Context);

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

	/**
	 * 释放
	 */
	virtual void Release(SSFObjectErrors &Errors);
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
