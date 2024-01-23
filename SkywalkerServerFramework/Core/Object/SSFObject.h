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
	SSF_PTR(SkywalkerServerFramework)
	SSFramework{};
};

class SSFObject
{

protected:
	SSFString ObjectClassName{};

private:
	SSF_PTR(SkywalkerServerFramework)
	SSFramework{};

	SSFObjectGUID ObjectGUID{SSF_OBJECT_INVALID_GUID};

public:
	SSFObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
	{
		SSFramework = InContext.SSFramework;

		// 生成ObjectGUID
		ObjectGUID = SSFramework->NewObjectGUID();
	};
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
		return SSFramework;
	};

	/**
	 * 获取ObjectGUID
	 */
	SSFObjectGUID GetObjectGUID()
	{
		return ObjectGUID;
	};

	/**
	 * 创建对象
	 */
	template <typename ObjectT, typename ContextT>
	SSF_PTR(ObjectT)
	NewObject(ContextT &InContext, SSFObjectErrors &InErrors)
	{
		return new ObjectT(InContext, InErrors);
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT>
	SSF_PTR(ObjectT)
	NewObject(SSFObjectErrors &InErrors)
	{
		SSFObjectContext ObjectContext;
		ObjectContext.SSFramework = SSFramework;
		return new ObjectT(ObjectContext, InErrors);
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT, typename ContextT>
	SSF_PTR(ObjectT)
	NewObject(ContextT &InContext)
	{
		return new ObjectT(InContext, SSFObjectErrors());
	}

	/**
	 * 创建对象
	 */
	template <typename ObjectT>
	SSF_PTR(ObjectT)
	NewObject()
	{
		SSFObjectContext ObjectContext;
		ObjectContext.SSFramework = SSFramework;
		return new ObjectT(ObjectContext, SSFObjectErrors());
	}
};

SSF_NAMESPACE_END

/**
 * 定义对象类
 */
#define SSF_OBJECT_CLASS(Class)                            \
public:                                                    \
	virtual const SSFString &GetObjectClassName() override \
	{                                                      \
		if (this->ObjectClassName.empty())                 \
		{                                                  \
			SSF_CLASS_NAME(this, this->ObjectClassName);   \
		}                                                  \
		return this->ObjectClassName;                      \
	};

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
