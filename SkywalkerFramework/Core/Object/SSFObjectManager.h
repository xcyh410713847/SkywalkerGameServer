/*************************************************************************
**文件: SkywalkerFramework\Core\Object\SSFObjectManager.h
**作者: shyfan
**日期: 2024/01/20 16:01:41
**功能: SSFObjectManager
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__

#include "Include/SFCore.h"

#include "Core/Object/SSFObject.h"

SF_NAMESPACE_BEGIN

SF_TEMPLATE_CLASS(SSFObject, Object)
class SSFObjectManager : public SSFObject
{
public:
    SSFObjectManager(SSFObjectContext &InContext, SFObjectErrors &InErrors)
        : SSFObject(InContext, InErrors) {};
    virtual ~SSFObjectManager() {};

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
     * 获取对象
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
     * 获取对象
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
    SFMap<SFObjectGUID, SF_PTR(Object)> ObjectMap;
};

SF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__
