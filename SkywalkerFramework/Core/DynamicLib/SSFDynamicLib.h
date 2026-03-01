/*************************************************************************
**文件: SkywalkerFramework\Core\DynamicLib\SSFDynamicLib.h
**作者: shyfan
**日期: 2023/08/12 16:07:47
**功能: 动态库
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__
#define __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__

#include "Include/SFCore.h"

#define SKYWALKER__DYNAMIC_LIB_PATH "./"

SF_NAMESPACE_BEGIN

class SFDynamicLib
{
public:
    SFDynamicLib(const SFString &InName);
    virtual ~SFDynamicLib();

    bool Load();
    bool Unload();

    const SFString &GetName() const;

    void *GetSymbol(const char *ProcName);

private:
    SFString Name;

    SKYWALKER_DYNAMIC_LIB_HANDLE Instance;
};

/**
 * SSFCDynamicLib 指针
 */
#define SSF_PTR_DYNAMIC_LIB SF_SHARED_PTR(SFDynamicLib)

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__
