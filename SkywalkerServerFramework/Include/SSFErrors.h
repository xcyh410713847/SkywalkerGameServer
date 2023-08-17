/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFErrors.h
**作者: shyfan
**日期: 2023/07/30 13:25:43
**功能: 错误码
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__

#include "SkywalkerErrors\SkywalkerErrors.h"

#include "SSFCore.h"

SKYWALKER_SF_NAMESPACE_BEGIN

enum ESkywalkerSFError
{
    SkywalkerSFError_Success = 0, // 成功
    SkywalkerSFError_Unknow,      // 未知错误

#pragma region 插件错误

    SkywalkerSFError_Plugin_Error = 100,            // 插件错误
    SkywalkerSFError_Plugin_Register_nullptr,       // 注册插件为空
    SkywalkerSFError_Plugin_Register_NameEmpty,     // 注册插件名称为空
    SkywalkerSFError_Plugin_Register_Repeat,        // 注册插件重复
    SkywalkerSFError_Plugin_Unregister_nullptr,     // 注销插件为空
    SkywalkerSFError_Plugin_Unregister_NameEmpty,   // 注销插件名称为空
    SkywalkerSFError_Plugin_Unregister_NotFound,    // 注销插件未找到
    SkywalkerSFError_Plugin_Load_Repeated,          // 插件重复加载
    SkywalkerSFError_Plugin_Load_DynamicLibNullptr, // 动态库为空
    SkywalkerSFError_Plugin_Load_Failed,            // 插件加载失败
    SkywalkerSFError_Plugin_Load_EntryNullptr,      // 插件入口为空

#pragma endregion 插件错误

#pragma region 模块错误

    SkywalkerSFError_Module_Error = 200,          // 模块错误
    SkywalkerSFError_Module_Register_nullptr,     // 注册模块为空
    SkywalkerSFError_Module_Register_NameEmpty,   // 注册模块名称为空
    SkywalkerSFError_Module_Register_Repeat,      // 注册模块重复
    SkywalkerSFError_Module_Unregister_nullptr,   // 注销模块为空
    SkywalkerSFError_Module_Unregister_NameEmpty, // 注销模块名称为空
    SkywalkerSFError_Module_Unregister_NotFound,  // 注销模块未找到

#pragma endregion 模块错误
};

// 对象错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<std::string> SSFObjectErrors;

// 插件错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> SSFPluginErrors;

// 模块错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> SSFModuleErrors;

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__
