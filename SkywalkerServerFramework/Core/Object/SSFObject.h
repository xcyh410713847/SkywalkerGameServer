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
#include "Include/SSFramework.h"

SSF_NAMESPACE_BEGIN

/**
 * 对象上下文
 */
struct SSFObjectContext
{
};

class SSFObject
{

protected:
	SSFString ObjectClassName{};

private:
	SSF_PTR(SSFObject)
	RootObject{};

	SSFObjectGUID ObjectGUID{SSF_OBJECT_INVALID_GUID};

public:
	SSFObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors){};
	SSFObject(){};
	virtual ~SSFObject(){};

	/**
	 * 释放
	 */
	virtual void Release(SSFObjectErrors &Errors)
	{
		delete this;
	};

	/**
	 * 获取类名
	 * @return 类名 const SSFString &
	 */
	virtual const SSFString &GetObjectClassName() = 0;

	/**
	 * 获取框架
	 */
	SSF_PTR(SkywalkerServerFramework)
	GetFramework()
	{
		return SSF_PTR_DYNAMIC_CAST(SkywalkerServerFramework)(GetRootObject());
	};

	/**
	 * 获取ObjectGUID
	 */
	SSFObjectGUID GetObjectGUID()
	{
		return ObjectGUID;
	};

private:
	/**
	 * 获取 RootObject
	 */
	SSF_PTR(SSFObject)
	GetRootObject()
	{
		if (SSF_PTR_INVALID(RootObject))
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
	SSF_PTR(ObjectT)
	NewObject(ContextT &InContext, SSFObjectErrors &InErrors)
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
	SSF_PTR(ObjectT)
	NewObject(SSFObjectErrors &InErrors)
	{
		static SSFObjectContext InContext{};
		return NewObject<ObjectT>(InContext, InErrors);
		;
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT, typename ContextT>
	SSF_PTR(ObjectT)
	NewObject(ContextT &InContext)
	{
		SSFObjectErrors Errors{};
		return NewObject<ObjectT>(InContext, Errors);
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT>
	SSF_PTR(ObjectT)
	NewObject()
	{
		SSFObjectErrors Errors{};
		return NewObject<ObjectT>(Errors);
	}

#pragma endregion NewObject
};

SSF_NAMESPACE_END

/**
 * 定义对象类
 * SSF_CLASS_NAME  中为什么要解指针？因为在宏定义中，如果不解指针，在Linux下获得的类名会多一个"P"
 */
#define SSF_OBJECT_CLASS(Class)                            \
public:                                                    \
	virtual const SSFString &GetObjectClassName() override \
	{                                                      \
		if (this->ObjectClassName.empty())                 \
		{                                                  \
			SSF_CLASS_NAME(*this, this->ObjectClassName);   \
		}                                                  \
		return this->ObjectClassName;                      \
	};

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
