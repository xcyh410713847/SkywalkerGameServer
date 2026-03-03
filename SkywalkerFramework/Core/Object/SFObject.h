/*************************************************************************
**文件: SkywalkerFramework\Core\Object\SFObject.h
**作者: shyfan
**日期: 2023/08/07 23:52:40
**功能: 对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__

#include "Include/SFCore.h"
#include "Include/SFErrors.h"
#include "Include/SFFramework.h"

SF_NAMESPACE_BEGIN

/**
 * 对象上下文
 */
struct SSFObjectContext
{
};

class SSFObject
{

protected:
	SFString ObjectClassName{};

private:
	SF_PTR(SSFObject)
	RootObject{};

	SFObjectGUID ObjectGUID{SF_OBJECT_INVALID_GUID};

public:
	SSFObject(SSFObjectContext &InContext, SFObjectErrors &InErrors) {};
	SSFObject() {};
	virtual ~SSFObject() {};

	/**
	 * 释放
	 */
	virtual void Release(SFObjectErrors &Errors)
	{
		delete this;
	};

	/**
	 * 获取类名
	 * @return 类名 const SSFString &
	 */
	virtual const SFString &GetObjectClassName()
	{
		if (ObjectClassName.empty())
		{
			SF_CLASS_NAME(*this, ObjectClassName);
		}

		return ObjectClassName;
	}

	/**
	 * 获取框架
	 */
	SF_PTR(SkywalkerFramework)
	GetFramework()
	{
		return SF_PTR_DYNAMIC_CAST(SkywalkerFramework)(GetRootObject());
	};

	/**
	 * 获取ObjectGUID
	 */
	SFObjectGUID GetObjectGUID()
	{
		return ObjectGUID;
	};

private:
	/**
	 * 获取 RootObject
	 */
	SF_PTR(SSFObject)
	GetRootObject()
	{
		if (SF_PTR_INVALID(RootObject))
		{
			RootObject = this;
		}

		return RootObject;
	};
#pragma region NewObject

public:
	/**
	 * 创建对象
	 */
	template <typename ObjectT, typename ContextT>
	SF_PTR(ObjectT)
	NewObject(ContextT &InContext, SFObjectErrors &InErrors)
	{
		auto Object = new ObjectT(InContext, InErrors);

		Object->RootObject = GetRootObject();
		Object->ObjectGUID = GetFramework()->NewObjectGUID();

		return Object;
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT>
	SF_PTR(ObjectT)
	NewObject(SFObjectErrors &InErrors)
	{
		static SSFObjectContext InContext{};
		return NewObject<ObjectT>(InContext, InErrors);
		;
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT, typename ContextT>
	SF_PTR(ObjectT)
	NewObject(ContextT &InContext)
	{
		SFObjectErrors Errors{};
		return NewObject<ObjectT>(InContext, Errors);
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT>
	SF_PTR(ObjectT)
	NewObject()
	{
		SFObjectErrors Errors{};
		return NewObject<ObjectT>(Errors);
	}

#pragma endregion NewObject
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
