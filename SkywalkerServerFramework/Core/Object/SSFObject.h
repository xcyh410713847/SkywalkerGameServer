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

class SSFObject
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
};

SSF_NAMESPACE_END

/**
 * 定义对象类
 */
#define SSF_OBJECT_CLASS(Class)                        \
public:                                                \
	const std::string GetObjectClassName()             \
	{                                                  \
		return SSF_CLASS_NAME(Class);                  \
	};                                                 \
                                                       \
	const SSFUInt &GetPoolSize() { return PoolSize; }; \
                                                       \
protected:                                             \
	SSFUInt PoolSize = 0;

/**
 * 定义对象类，带对象池大小
 */
#define SSF_OBJECT_CLASS_POOL(Class, InPoolSize)       \
public:                                                \
	const std::string GetObjectClassName()             \
	{                                                  \
		return SSF_CLASS_NAME(Class);                  \
	};                                                 \
                                                       \
	const SSFUInt &GetPoolSize() { return PoolSize; }; \
                                                       \
protected:                                             \
	SSFUInt PoolSize = InPoolSize;

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
