/*************************************************************************
**文件: skywalker-derived.h
**作者: shyfan
**日期: 2023/07/26 20:26:49
**功能: 继承检查
*************************************************************************/

#ifndef __SKYWALKER_DERIVED_H__
#define __SKYWALKER_DERIVED_H__

namespace Skywalker
{
    namespace Derived
    {
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
    }
}

#define SKYWALKER_IS_DERIVED(DerivedType, BaseType) Skywalker::Derived::TIsDerived<DerivedType, BaseType>::Result

#endif // __SKYWALKER_DERIVED_H__
