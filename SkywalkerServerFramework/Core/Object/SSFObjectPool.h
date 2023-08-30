/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObjectPool.h
**作者: shyfan
**日期: 2023/08/30 16:24:27
**功能: 对象池
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_OBJECT_POOL_H__
#define __SKYWALKER_SERVER_FRAMEWORK_OBJECT_POOL_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFObjectPool : public SSFObject
{
public:
    SSFObjectPool(SSFObjectCreatorContext &Context);
    virtual ~SSFObjectPool();

    /**
     * 从对象池中获取对象
     */
    SSFObject *GetObject();

    /**
     * 回收对象
     */
    void RecycleObject(SSFObject *Object);

    /**
     * 清空对象池
     */
    void Clear();

private:
    std::list<SSFObject *> ObjectVector;
};

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_OBJECT_POOL_H__
