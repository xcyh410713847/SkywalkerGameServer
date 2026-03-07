/*************************************************************************
**文件: SkywalkerFramework\Core\Object\SFObjectManager.h
**作者: shyfan
**日期: 2024/01/20 16:01:41
**功能: SSFObjectManager
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__

#include "Include/SFCore.h"

#include "Core/Object/SFObject.h"

SF_NAMESPACE_BEGIN

SF_TEMPLATE_CLASS(SSFObject, Object)
/**
 * 对象管理器（模板）
 *
 * 职责：
 * 1. 以 ObjectGUID 为键维护对象索引
 * 2. 提供对象添加、移除、查找能力
 * 3. 在释放阶段统一释放已托管对象
 */
class SSFObjectManager : public SSFObject
{
public:
    /** 构造函数 */
    SSFObjectManager(SSFObjectContext &InContext, SFObjectErrors &InErrors)
        : SSFObject(InContext, InErrors) {};
    /** 析构函数 */
    virtual ~SSFObjectManager() {};

    /**
     * 释放对象管理器
     * 说明：会先释放 ObjectMap 中所有对象，再释放管理器自身。
     */
    virtual void Release(SFObjectErrors &Errors) override
    {
        SF_COMMON_ITERATOR(IterObject, ObjectMap)
        {
            if (SF_PTR_INVALID(IterObject->second))
            {
                continue;
            }
            IterObject->second->Release(Errors);
        }
        ObjectMap.clear();

        SSFObject::Release(Errors);
    };

    /**
     * 加入对象
     * @param Errors 错误输出对象
     * @param AddObject 待加入对象
     * 说明：会校验对象指针、GUID 有效性及重复添加。
     */
    virtual void AddObject(SFObjectErrors &Errors, SF_PTR(Object) AddObject)
    {
        if (!SF_PTR_VALID(AddObject))
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_Nullptr, "ObjectManager AddObject nullptr");
            return;
        }

        SFObjectGUID objectGUID = AddObject->GetObjectGUID();
        if (SF_OBJECT_GUID_INVALID(objectGUID))
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_GUID_Invalid, "ObjectManager AddObject GUID Invalid");
            return;
        }

        if (ObjectMap.find(objectGUID) != ObjectMap.end())
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_Add_Repeat, "ObjectManager AddObject Repeat");
            return;
        }

        ObjectMap.insert(std::make_pair(objectGUID, AddObject));
    }

    /**
     * 移除对象
     * @param Errors 错误输出对象
     * @param RemoveObject 待移除对象
     * 说明：按对象 GUID 从管理器中移除，不负责释放对象内存。
     */
    virtual void RemoveObject(SFObjectErrors &Errors, SF_PTR(Object) RemoveObject)
    {
        if (!SF_PTR_VALID(RemoveObject))
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_Nullptr, "ObjectManager RemoveObject nullptr");
            return;
        }

        SFObjectGUID objectGUID = RemoveObject->GetObjectGUID();
        if (SF_OBJECT_GUID_INVALID(objectGUID))
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_GUID_Invalid, "ObjectManager RemoveObject GUID Invalid");
            return;
        }

        auto iter = ObjectMap.find(objectGUID);
        if (iter == ObjectMap.end())
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_Add_Repeat, "ObjectManager RemoveObject NotFound");
            return;
        }

        ObjectMap.erase(iter);
    }

    /**
     * 查找对象（带错误输出）
     * @param Errors 错误输出对象
     * @param InObjectGUID 目标对象 GUID
     * @return 找到返回对象指针，失败返回 nullptr
     */
    virtual SF_PTR(Object) FindObject(SFObjectErrors &Errors, const SFObjectGUID &InObjectGUID)
    {
        if (SF_OBJECT_GUID_INVALID(InObjectGUID))
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_GUID_Invalid, "ObjectManager GetObject GUID Invalid");
            return nullptr;
        }

        auto iter = ObjectMap.find(InObjectGUID);
        if (iter == ObjectMap.end())
        {
            SF_ERROR_DESC_TRACE(Errors, ESFError::Object_Not_Found, "ObjectManager GetObject NotFound");
            return nullptr;
        }

        return iter->second;
    }

    /**
     * 查找对象（轻量重载，无错误输出）
     * @param InObjectGUID 目标对象 GUID
     * @return 找到返回对象指针，失败返回 nullptr
     */
    virtual SF_PTR(Object) FindObject(const SFObjectGUID &InObjectGUID)
    {
        if (SF_OBJECT_GUID_INVALID(InObjectGUID))
        {
            return nullptr;
        }

        auto iter = ObjectMap.find(InObjectGUID);
        if (iter == ObjectMap.end())
        {
            return nullptr;
        }

        return iter->second;
    }

private:
    /** 对象索引表（Key: ObjectGUID，Value: Object 指针） */
    SFMap<SFObjectGUID, SF_PTR(Object)> ObjectMap;
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__
