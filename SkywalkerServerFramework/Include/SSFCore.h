/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFCore.h
**作者: shyfan
**日期: 2023/08/02 17:16:32
**功能: 核心头文件
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_CORE_H__

#include <iostream>

#pragma region Common Macro

#define SKYWALKER_SF_NAMESPACE_BEGIN \
    namespace Skywalker              \
    {                                \
        namespace ServerFramework    \
        {

#define SKYWALKER_SF_NAMESPACE_END \
    } /* ServerFramework */        \
    } /* Skywalker */

#define SKYWALKER_SF_NAMESPACE_USE using namespace Skywalker::ServerFramework;

#define SKYWALKER_SF_API extern "C"

#pragma endregion Common Macro

SKYWALKER_SF_API static bool SkywalkerServerFrameworkInit()
{
    std::cout << "SkywalkerServerFrameworkInit" << std::endl;
    return true;
}

#endif // __SKYWALKER_SERVER_FRAMEWORK_CORE_H__
