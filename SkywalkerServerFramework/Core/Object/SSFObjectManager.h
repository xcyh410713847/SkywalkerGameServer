/*************************************************************************
**文件: SkywalkerServerFramework\Core\Object\SSFObjectManager.h
**作者: shyfan
**日期: 2024/01/20 16:01:41
**功能: SSFObjectManager
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__
#define __SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__

#include "Include/SSFCore.h"

#include "Core/Object/SSFObject.h"

SSF_NAMESPACE_BEGIN

SSF_TEMPLATE_CLASS(SSFObject, Object)
class SSFObjectManager : public SSFObject
{
    SSF_OBJECT_CLASS(SSFObjectManager)

public:
    SSFObjectManager(SSFObjectContext &InContext, SSFObjectErrors &InErrors)
        : SSFObject(InContext, InErrors){};
    virtual ~SSFObjectManager(){};

    /**
     * 加入对象
     */
    virtual void AddObject(SSFObjectErrors &Errors, SSF_PTR(Object) AddObject)
    {
        if (!SSF_PTR_VALID(AddObject))
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_Nullptr, "ObjectManager AddObject nullptr");
            return;
        }

        SSFObjectGUID objectGUID = AddObject->GetObjectGUID();
        if (SSF_OBJECT_GUID_INVALID(objectGUID))
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_GUID_Invalid, "ObjectManager AddObject GUID Invalid");
            return;
        }

        if (ObjectMap.find(objectGUID) != ObjectMap.end())
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_Add_Repeat, "ObjectManager AddObject Repeat");
            return;
        }

        ObjectMap.insert(std::make_pair(objectGUID, AddObject));
    }

    /**
     * 移除对象
     */
    virtual void RemoveObject(SSFObjectErrors &Errors, SSF_PTR(Object) RemoveObject)
    {
        if (!SSF_PTR_VALID(RemoveObject))
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_Nullptr, "ObjectManager RemoveObject nullptr");
            return;
        }

        SSFObjectGUID objectGUID = RemoveObject->GetObjectGUID();
        if (SSF_OBJECT_GUID_INVALID(objectGUID))
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_GUID_Invalid, "ObjectManager RemoveObject GUID Invalid");
            return;
        }

        auto iter = ObjectMap.find(objectGUID);
        if (iter == ObjectMap.end())
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_Add_Repeat, "ObjectManager RemoveObject NotFound");
            return;
        }

        ObjectMap.erase(iter);
    }

    /**
     * 获取对象
     */
    virtual SSF_PTR(Object) FindObject(SSFObjectErrors &Errors, const SSFObjectGUID& InObjectGUID)
    {
        if (SSF_OBJECT_GUID_INVALID(InObjectGUID))
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_GUID_Invalid, "ObjectManager GetObject GUID Invalid");
            return nullptr;
        }

        auto iter = ObjectMap.find(InObjectGUID);
        if (iter == ObjectMap.end())
        {
            SSF_ERROR_DESC_TRACE(Errors, SkywalkerSFError_Object_Add_Repeat, "ObjectManager GetObject NotFound");
            return nullptr;
        }

        return iter->second;
    }

    /**
     * 获取对象
     */
    virtual SSF_PTR(Object) FindObject(const SSFObjectGUID& InObjectGUID)
    {
        if (SSF_OBJECT_GUID_INVALID(InObjectGUID))
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
    SSFMap<SSFObjectGUID, SSF_PTR(Object)> ObjectMap;
};

SSF_NAMESPACE_END

#endif //__SKYWALKER_SERVER_FRAMEWORK_SSFObjectManager_H__
