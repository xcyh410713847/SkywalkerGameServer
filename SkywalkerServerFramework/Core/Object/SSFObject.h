/*************************************************************************
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

SSF_NAMESPACE_BEGIN

class CSkywalkerServerFramework;

/**
 * 对象创建上下文
 */
struct SSFObjectContext
{
	SSF_PTR(CSkywalkerServerFramework)
	SSFramework;
};

class SSFObject : public SSFObjectSimple
{
public:
	SSFObject();
	SSFObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors);
	virtual ~SSFObject();

	/**
	 * 创建对象
	 */
	template <typename T>
	SSF_SHARED_PTR(T)
	NewSharedObject();

	/**
	 * 创建对象
	 */
	template <typename T>
	SSF_SHARED_PTR(T)
	NewSharedObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors);

	/**
	 * 创建对象
	 */
	template <typename T>
	SSF_PTR(T)
	NewObject();

	/**
	 * 创建对象
	 */
	template <typename T>
	SSF_PTR(T)
	NewObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors);

private:
	SSFObjectContext Context;

#pragma region Object

public:
	/**
	 * 调用循序
	 * Create -> Init -> Awake -> Start -> Tick -> Stop -> Sleep -> Destroy -> Release
	 */

	virtual void
	Create(SSFObjectErrors &Errors, SSFObjectContext &Context);

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

#pragma endregion Object
};

SSF_NAMESPACE_END

/**
 * 定义对象类
 */
#define SSF_OBJECT_CLASS(Class)                                 \
public:                                                         \
	virtual const std::string GetObjectClassName() override     \
	{                                                           \
		return SSF_CLASS_NAME(Class);                           \
	};                                                          \
                                                                \
	const SSFUInt &GetPoolSize() override { return PoolSize; }; \
                                                                \
protected:                                                      \
	SSFUInt PoolSize = 0;

/**
 * 定义对象类，带对象池大小
 */
#define SSF_OBJECT_CLASS_POOL(Class, InPoolSize)                \
public:                                                         \
	virtual const std::string GetObjectClassName() override     \
	{                                                           \
		return SSF_CLASS_NAME(Class);                           \
	};                                                          \
                                                                \
	const SSFUInt &GetPoolSize() override { return PoolSize; }; \
                                                                \
protected:                                                      \
	SSFUInt PoolSize = InPoolSize;

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
