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
 * 对象上下文
 */
struct SSFObjectContext
{
	SSF_PTR(CSkywalkerServerFramework)
	SSFramework{};
};

class SSFObject
{

protected:
	SSFString ObjectClassName{};
	SSFObjectContext ObjectContext;

public:
	SSFObject(){};
	SSFObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
	{
		ObjectContext = InContext;
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
	 * @return 类名 const std::string &
	 */
	virtual const std::string &
	GetObjectClassName() = 0;
};

SSF_NAMESPACE_END

/**
 * 定义对象类
 */
#define SSF_OBJECT_CLASS(Class)                              \
public:                                                      \
	virtual const std::string &GetObjectClassName() override \
	{                                                        \
		if (ObjectClassName.empty())                         \
		{                                                    \
			SSF_CLASS_NAME(this, ObjectClassName);           \
		}                                                    \
		return ObjectClassName;                              \
	};

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
