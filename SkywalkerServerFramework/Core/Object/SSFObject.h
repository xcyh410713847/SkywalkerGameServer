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
	SSFramework{};
};

class SSFObject
{
public:
	static const std::string GetObjectClassName()
	{
		return SSF_CLASS_NAME(SSFObject);
	};

public:
	SSFObject(){};
	SSFObject(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
	{
		Context = InContext;
	};
	virtual ~SSFObject(){};

private:
	SSFObjectContext Context;
};

SSF_NAMESPACE_END

/**
 * 定义对象类
 */
#define SSF_OBJECT_CLASS(Class)                   \
public:                                           \
	static const std::string GetObjectClassName() \
	{                                             \
		return SSF_CLASS_NAME(Class);             \
	};

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_H__
