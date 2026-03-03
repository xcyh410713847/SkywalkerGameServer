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

/**
 * 动态库封装对象
 *
 * 职责：
 * - 依据库名加载/卸载动态库实例
 * - 提供按符号名获取函数地址能力
 *
 * 说明：
 * - 库文件路径前缀由 `SKYWALKER__DYNAMIC_LIB_PATH` 决定
 * - 实际句柄类型由平台宏 `SKYWALKER_DYNAMIC_LIB_HANDLE` 定义
 */
class SFDynamicLib
{
public:
    /**
     * 构造动态库对象
     * @param InName 动态库名称（不含路径前缀）
     */
    SFDynamicLib(const SFString &InName);

    /**
     * 析构函数
     * 约定：对象销毁前应保证库句柄已正确释放
     */
    virtual ~SFDynamicLib();

    /**
     * 加载动态库
     * @return true: 加载成功；false: 加载失败
     */
    bool Load();

    /**
     * 卸载动态库
     * @return true: 卸载成功；false: 卸载失败
     */
    bool Unload();

    /**
     * 获取动态库名称
     * @return 构造时传入的库名引用
     */
    const SFString &GetName() const;

    /**
     * 获取导出符号地址
     * @param ProcName 导出函数/符号名称
     * @return 成功返回符号地址，失败返回 nullptr
     */
    void *GetSymbol(const char *ProcName);

private:
    /** 动态库名称 */
    SFString Name;

    /** 动态库实例句柄 */
    SKYWALKER_DYNAMIC_LIB_HANDLE Instance;
};

/**
 * SFDynamicLib 智能指针别名
 */
#define SSF_PTR_DYNAMIC_LIB SF_SHARED_PTR(SFDynamicLib)

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_DYNAMIC_LIB_H__
