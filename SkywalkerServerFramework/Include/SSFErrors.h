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

    /**
     * 插件错误
     */
    SkywalkerSFError_Register_Plugin_nullptr,       // 注册插件为空
    SkywalkerSFError_Register_Plugin_NameEmpty,     // 注册插件名称为空
    SkywalkerSFError_Register_Plugin_Repeat,        // 注册插件重复
    SkywalkerSFError_Unregister_Plugin_nullptr,     // 注销插件为空
    SkywalkerSFError_Unregister_Plugin_NameEmpty,   // 注销插件名称为空
    SkywalkerSFError_Unregister_Plugin_NotFound,    // 注销插件未找到
    SkywalkerSFError_Load_Plugin_Repeated,          // 插件重复加载
    SkywalkerSFError_Load_Plugin_DynamicLibNullptr, // 动态库为空
    SkywalkerSFError_Load_Plugin_Failed,            // 插件加载失败
    SkywalkerSFError_Load_Plugin_EntryNullptr,      // 插件入口为空

    /**
     * 模块错误
     */
    SkywalkerSFError_Register_Module_nullptr,     // 注册模块为空
    SkywalkerSFError_Register_Module_NameEmpty,   // 注册模块名称为空
    SkywalkerSFError_Register_Module_Repeat,      // 注册模块重复
    SkywalkerSFError_Unregister_Module_nullptr,   // 注销模块为空
    SkywalkerSFError_Unregister_Module_NameEmpty, // 注销模块名称为空
    SkywalkerSFError_Unregister_Module_NotFound,  // 注销模块未找到
};

// 对象错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<std::string> SSFObjectErrors;

// 插件错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> SSFPluginErrors;

// 模块错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> SSFModuleErrors;

SKYWALKER_SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__
