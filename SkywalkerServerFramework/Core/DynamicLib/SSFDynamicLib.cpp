﻿/*************************************************************************
**文件: SkywalkerServerFramework\Core\DynamicLib\SSFDynamicLib.cpp
**作者: shyfan
**日期: 2023/08/12 16:07:58
**功能: 动态库
*************************************************************************/

#include "SSFDynamicLib.h"

SSF_NAMESPACE_USING

SSFDynamicLib::SSFDynamicLib(const SSFString &InName)
{
    Name = InName;
    Instance = NULL;

    Name.append(SKYWALKER_DYNAMIC_LIB_EXT);
}

SSFDynamicLib::~SSFDynamicLib()
{
}

bool SSFDynamicLib::Load()
{
    SSFString Path = SKYWALKER__DYNAMIC_LIB_PATH;
    Path.append(Name);

    Instance = SKYWALKER_DYNAMIC_LIB_LOAD(Path.c_str());

    return Instance != nullptr;
}

bool SSFDynamicLib::Unload()
{
    SKYWALKER_DYNAMIC_LIB_UNLOAD(Instance);
    return true;
}

void *SSFDynamicLib::GetSymbol(const char *ProcName)
{
    return (SKYWALKER_DYNAMIC_LIB_HANDLE)SKYWALKER_DYNAMIC_LIB_GET_SYMBOL(Instance, ProcName);
}
