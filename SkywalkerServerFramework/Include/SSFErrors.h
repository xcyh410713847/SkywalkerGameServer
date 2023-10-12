/*************************************************************************
**文件: SkywalkerServerFramework\Include\SSFErrors.h
**作者: shyfan
**日期: 2023/07/30 13:25:43
**功能: 错误码
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__
#define __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__

#include "SkywalkerErrors/SkywalkerErrors.h"

#include "SSFCore.h"

SSF_NAMESPACE_BEGIN

enum ESkywalkerSFError
{
    SkywalkerSFError_Success = 0, // 成功
    SkywalkerSFError_Unknow,      // 未知错误

#pragma region 插件错误

    SkywalkerSFError_Plugin_Error = 100,            // 插件错误
    SkywalkerSFError_Plugin_Register_Failed,        // 注册插件失败
    SkywalkerSFError_Plugin_Register_nullptr,       // 注册插件为空
    SkywalkerSFError_Plugin_Register_NameEmpty,     // 注册插件名称为空
    SkywalkerSFError_Plugin_Register_Repeat,        // 注册插件重复
    SkywalkerSFError_Plugin_Unregister_Failed,      // 注销插件失败
    SkywalkerSFError_Plugin_Unregister_nullptr,     // 注销插件为空
    SkywalkerSFError_Plugin_Unregister_NameEmpty,   // 注销插件名称为空
    SkywalkerSFError_Plugin_Unregister_NotFound,    // 注销插件未找到
    SkywalkerSFError_Plugin_Load_ConfigNullptr,     // 插件配置为空
    SkywalkerSFError_Plugin_Load_Repeated,          // 插件重复加载
    SkywalkerSFError_Plugin_Load_DynamicLibNullptr, // 动态库为空
    SkywalkerSFError_Plugin_Load_Failed,            // 插件加载失败
    SkywalkerSFError_Plugin_Load_EntryNullptr,      // 插件入口为空

#pragma endregion 插件错误

#pragma region 模块错误

    SkywalkerSFError_Module_Error = 200,          // 模块错误
    SkywalkerSFError_Module_Register_Failed,      // 注册模块失败
    SkywalkerSFError_Module_Register_nullptr,     // 注册模块为空
    SkywalkerSFError_Module_Register_NameEmpty,   // 注册模块名称为空
    SkywalkerSFError_Module_Register_Repeat,      // 注册模块重复
    SkywalkerSFError_Module_Unregister_Failed,    // 注销模块失败
    SkywalkerSFError_Module_Unregister_nullptr,   // 注销模块为空
    SkywalkerSFError_Module_Unregister_NameEmpty, // 注销模块名称为空
    SkywalkerSFError_Module_Unregister_NotFound,  // 注销模块未找到

#pragma endregion 模块错误

#pragma region 对象错误

    SkywalkerSFError_Object_Error = 300, // 模块错误
    SkywalkerSFError_Object_Init_Failed, // 对象初始化失败

#pragma endregion 对象错误

#pragma region 网络错误

    SkywalkerSFError_Network_Error = 400,         // 网络错误
    SkywalkerSFError_Network_Init_Failed,         // 网络初始化失败
    SkywalkerSFError_Network_Start_Failed,        // 网络启动失败
    SkywalkerSFError_Network_Socket_CreateFailed, // 创建套接字失败
    SkywalkerSFError_Network_Socket_BindFailed,   // 绑定套接字失败
    SkywalkerSFError_Network_Socket_ListenFailed, // 监听套接字失败
    SkywalkerSFError_Network_Socket_SetFailed,    // 设置套接字失败

#pragma endregion 网络错误

};

struct SSFError
{
    ESkywalkerSFError Error;
    std::string ErrorDesc;
    SSFError()
        : Error(SkywalkerSFError_Unknow),
          ErrorDesc("")
    {
    }

    SSFError(ESkywalkerSFError InError)
        : Error(InError),
          ErrorDesc("")
    {
    }

    SSFError(ESkywalkerSFError InError, std::string InErrorDesc)
        : Error(InError), ErrorDesc(InErrorDesc) {}

    std::string GetContent()
    {
        std::string content = "Error: " + std::to_string(Error);
        if (!ErrorDesc.empty())
        {
            content += ", ErrorDesc: " + ErrorDesc;
        }
        return content;
    }
};

// 错误
typedef SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<SSFError> SkywalkerSFErrors;

// 对象错误
typedef SkywalkerSFErrors SSFObjectErrors;

// 插件错误
typedef SkywalkerSFErrors SSFPluginErrors;

// 模块错误
typedef SkywalkerSFErrors SSFModuleErrors;

// 网络错误
typedef SkywalkerSFErrors SSFNetworkErrors;

#pragma region 错误宏

/**
 * 不带堆栈的错误
 */
#define SSF_ERROR(CppErrors, Error) \
    SSFError error(Error);          \
    SKYWALKER_ERRORS_WRAP(CppErrors, error)

/**
 * 不带堆栈的错误
 */
#define SSF_ERROR_DESC(CppErrors, Error, ErrorDesc) \
    SSFError error(Error, ErrorDesc);               \
    SKYWALKER_ERRORS_WRAP(CppErrors, error)

/**
 * 带堆栈的错误
 */
#define SSF_ERROR_TRACE(CppErrors, Error) \
    SSFError error(Error);                \
    SKYWALKER_ERRORS_WRAP_TRACE(CppErrors, Error)

/**
 * 带堆栈的错误
 */
#define SSF_ERROR_DESC_TRACE(CppErrors, Error, ErrorDesc) \
    SSFError error(Error, ErrorDesc);                     \
    SKYWALKER_ERRORS_WRAP_TRACE(CppErrors, error)

#pragma endregion 错误宏

SSF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_ERRORS_H__
