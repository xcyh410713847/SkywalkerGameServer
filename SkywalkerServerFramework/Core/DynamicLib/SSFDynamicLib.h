/*************************************************************************
**文件: SkywalkerServerFramework\Core\DynamicLib\SSFDynamicLib.h
**作者: shyfan
**日期: 2023/08/12 16:07:47
**功能: 动态库
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__
#define __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__

#include "Include\SSFCore.h"

#define SKYWALKER__DYNAMIC_LIB_PATH "./"

SKYWALKER_SF_NAMESPACE_BEGIN

class SSFCDynamicLib
{
public:
    SSFCDynamicLib(const std::string &InName);
    virtual ~SSFCDynamicLib();

    bool Load();
    bool Unload();

    const std::string &GetName() const;

    void *GetSymbol(const char *ProcName);

private:
    std::string Name;

    SKYWALKER__DYNAMIC_LIB_HANDLE Instance;
};

SKYWALKER_SF_NAMESPACE_END

// SSFCDynamicLib 智能指针
typedef SSFSharedPtr<SKYWALKER_SF_NAMESPACE::SSFCDynamicLib> SSFSharedPtr_DynamicLib;

#endif // __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__
