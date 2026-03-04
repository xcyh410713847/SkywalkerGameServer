/*************************************************************************
**文件: SkywalkerTools\SkywalkerDerived\SkywalkerDerived.h
**作者: shyfan
**日期: 2023/07/30 12:39:16
**功能: 派生检查
*************************************************************************/

#ifndef __SKYWALKER_DERIVED_H__
#define __SKYWALKER_DERIVED_H__

/** 派生检测命名空间宏 */
#define SKYWALKER_DERIVED_NAMESPACE Skywalker::Derived
#define SKYWALKER_DERIVED_NAMESPACE_BEGIN \
    namespace SKYWALKER_DERIVED_NAMESPACE \
    {
#define SKYWALKER_DERIVED_NAMESPACE_END } // namespace SKYWALKER_DERIVED_NAMESPACE

SKYWALKER_DERIVED_NAMESPACE_BEGIN

template <typename DerivedType, typename BaseType>
class TIsDerived
{
public:
    /**
     * 当可将 DerivedType* 转换为 BaseType* 时优先匹配该重载
     */
    static int AnyFunction(BaseType *base)
    {
        return 1;
    }

    /**
     * 兜底重载
     */
    static char AnyFunction(void *t2)
    {
        return 0;
    }

    /**
     * Result = true 表示 DerivedType 继承自 BaseType
     */
    enum
    {
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType *)nullptr))),
    };
};

/**
 * 继承判断宏
 * 返回 1/0（可在静态断言或编译期判断中使用）
 */
#define SKYWALKER_IS_DERIVED(DerivedType, BaseType) SKYWALKER_DERIVED_NAMESPACE::TIsDerived<DerivedType, BaseType>::Result

SKYWALKER_DERIVED_NAMESPACE_END

#endif // __SKYWALKER_DERIVED_H__
