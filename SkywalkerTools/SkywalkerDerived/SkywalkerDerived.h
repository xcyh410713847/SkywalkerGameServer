/*************************************************************************
**文件: SkywalkerTools\SkywalkerDerived\SkywalkerDerived.h
**作者: shyfan
**日期: 2023/07/30 12:39:16
**功能: 派生检查
*************************************************************************/

#ifndef __SKYWALKER_DERIVED_H__
#define __SKYWALKER_DERIVED_H__

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
    static int AnyFunction(BaseType *base)
    {
        return 1;
    }

    static char AnyFunction(void *t2)
    {
        return 0;
    }

    enum
    {
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType *)NULL))),
    };
};

#define SKYWALKER_IS_DERIVED(DerivedType, BaseType) SKYWALKER_DERIVED_NAMESPACE::TIsDerived<DerivedType, BaseType>::Result

SKYWALKER_DERIVED_NAMESPACE_END

#endif // __SKYWALKER_DERIVED_H__
