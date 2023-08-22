/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFSingletonObject.h
**作者: shyfan
**日期: 2023/08/22 14:25:37
**功能: 单例对象
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SINGLETON_OBJECT_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SINGLETON_OBJECT_H__

#include "SkywalkerSingleton/SkywalkerSingleton.h"

#include "Include/SSFCore.h"

#include "SSFObject.h"

SKYWALKER_SF_NAMESPACE_BEGIN

/**
 * 单例对象
 */
class SSFSingletonObject : public SSFObject
{
    SKYWALKER_SINGLETON_DECLARE(SSFSingletonObject);
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_SINGLETON_OBJECT_H__
