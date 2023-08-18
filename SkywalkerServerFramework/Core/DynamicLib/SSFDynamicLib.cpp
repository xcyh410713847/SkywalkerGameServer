/*************************************************************************
**文件: SkywalkerServerFramework\Core\DynamicLib\SSFDynamicLib.cpp
**作者: shyfan
**日期: 2023/08/12 16:07:58
**功能: 动态库
*************************************************************************/

#include "SSFDynamicLib.h"

SKYWALKER_SF_NAMESPACE_USE

SSFCDynamicLib::SSFCDynamicLib(const std::string &InName)
{
    Name = InName;
    Instance = NULL;

    Name.append(SKYWALKER_SF_DYNAMIC_LIB_EXT);
}

SSFCDynamicLib::~SSFCDynamicLib()
{
}

bool SSFCDynamicLib::Load()
{
    std::string Path = SKYWALKER__DYNAMIC_LIB_PATH;
    Path.append(Name);

    Instance = SKYWALKER_SF_DYNAMIC_LIB_LOAD(Path.c_str());

    return Instance != nullptr;
}

bool SSFCDynamicLib::Unload()
{
    SKYWALKER_SF_DYNAMIC_LIB_UNLOAD(Instance);
    return true;
}

void *SSFCDynamicLib::GetSymbol(const char *ProcName)
{
    return (SKYWALKER_SF_DYNAMIC_LIB_HANDLE)SKYWALKER_SF_DYNAMIC_LIB_GET_SYMBOL(Instance, ProcName);
}
