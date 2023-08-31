/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObject.h
**作者: shyfan
**日期: 2023/08/07 23:52:40
**功能: 对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__

#include "SkywalkerPool/SkywalkerPool.h"

#include "Include/SSFCore.h"
#include "Include/SSFErrors.h"

#include "Core/Map/SSFMap.h"

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

class SSFObject
{
#pragma region NewObject

private:
	static SSFMap<std::string, SKYWALKER_POOL_PTR(SSFObject)> ObjectPoolMap;

public:
	template <typename T, typename... Params>
	static SKYWALKER_SF_PTR(T) NewObject(Params... param)
	{
		// 从对象池中获取对象
		std::string ClassName = typeid(T).name();
		auto Iterator = SSFObject::ObjectPoolMap.find(ClassName);
		if (Iterator != SSFObject::ObjectPoolMap.end())
		{
			SKYWALKER_POOL_PTR(SSFObject)
			ObjectPool = Iterator->second;
			if (SKYWALKER_SF_PTR_VALID(ObjectPool))
			{
				SSFObject *Object = ObjectPool->Get();
				if (SKYWALKER_SF_PTR_VALID(Object))
				{
					return (T *)Object;
				}
			}
		}

		return new T(param...);
	}

	static bool RemoveObject(SKYWALKER_SF_PTR_OBJECT Object)
	{
		if (!SKYWALKER_SF_PTR_VALID(Object))
		{
			return false;
		}

		// 回收对象
		std::string ClassName = typeid(Object).name();
		auto Iterator = SSFObject::ObjectPoolMap.find(ClassName);
		if (Iterator == SSFObject::ObjectPoolMap.end())
		{
			return false;
		}

		SKYWALKER_POOL_PTR(SSFObject)
		ObjectPool = Iterator->second;
		if (!SKYWALKER_SF_PTR_VALID(ObjectPool))
		{
			return false;
		}

		ObjectPool->Recycle(Object);

		return true;
	}

#pragma endregion NewObject

public:
	SSFObject();
	virtual ~SSFObject();

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

#define SSF_NEW_OBJECT(T, ...) SKYWALKER_SF_NAMESPACE::SSFObject::NewObject<T>(__VA_ARGS__)

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
