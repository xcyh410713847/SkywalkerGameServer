﻿/*************************************************************************
**文件: SkywalkerServerFramework\Core\DynamicLib\SSFDynamicLib.h
**作者: shyfan
**日期: 2023/08/12 16:07:47
**功能: 动态库
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__
#define __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__

#include "Include/SSFCore.h"

#define SKYWALKER__DYNAMIC_LIB_PATH "./"

SSF_NAMESPACE_BEGIN

class SSFDynamicLib
{
public:
    SSFDynamicLib(const SSFString &InName);
    virtual ~SSFDynamicLib();

    bool Load();
    bool Unload();

    const SSFString &GetName() const;

    void *GetSymbol(const char *ProcName);

private:
    SSFString Name;

    SKYWALKER_DYNAMIC_LIB_HANDLE Instance;
};

/**
 * SSFCDynamicLib 指针
 */
#define SSF_PTR_DYNAMIC_LIB SSF_SHARED_PTR(SSFDynamicLib)

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__
