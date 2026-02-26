/*************************************************************************
**文件: SkywalkerTools\SkywalkerTest\SkywalkerToolsTest.h
**作者: shyfan
**日期: 2026/02/26
**功能: SkywalkerTools 单元测试
*************************************************************************/

#ifndef __SKYWALKER_TOOLS_TEST_H__
#define __SKYWALKER_TOOLS_TEST_H__

#include "SkywalkerTest/SkywalkerTest.h"
#include "SkywalkerErrors/SkywalkerErrors.h"
#include "SkywalkerPool/SkywalkerPool.h"
#include "SkywalkerSingleton/SkywalkerSingleton.h"

SKYWALKER_TEST_SUITE(SkywalkerToolsErrors)

#pragma region SkywalkerErrors Tests

bool TestErrors_Empty()
{
    SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> Errors;
    SKYWALKER_TEST_ASSERT_FALSE(Errors.IsValid());
    return true;
}

bool TestErrors_AddError()
{
    SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> Errors;
    Errors.AddError(100, "test error");
    SKYWALKER_TEST_ASSERT_TRUE(Errors.IsValid());
    SKYWALKER_TEST_ASSERT_EQ(1, (int)Errors.GetErrors().size());
    return true;
}

bool TestErrors_GetFirstError()
{
    SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> Errors;
    Errors.AddError(100, "error1");
    Errors.AddError(200, "error2");
    SKYWALKER_TEST_ASSERT_EQ(100, Errors.GetFirstError());
    return true;
}

bool TestErrors_GetErrors()
{
    SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> Errors;
    Errors.AddError(100, "error1");
    Errors.AddError(200, "error2");
    auto ErrList = Errors.GetErrors();
    SKYWALKER_TEST_ASSERT_EQ(2, (int)ErrList.size());
    SKYWALKER_TEST_ASSERT_EQ(100, ErrList[0]);
    SKYWALKER_TEST_ASSERT_EQ(200, ErrList[1]);
    return true;
}

bool TestErrors_ErrorTrace()
{
    SKYWALKER_ERRORS_NAMESPACE::CSkywalkerErrors<int> Errors;
    Errors.AddError(100, "trace1");
    auto TraceList = Errors.GetErrorTrace();
    SKYWALKER_TEST_ASSERT_EQ(1, (int)TraceList.size());
    SKYWALKER_TEST_ASSERT_EQ("trace1", TraceList[0]);
    return true;
}

#pragma endregion

SKYWALKER_TEST_SUITE(SkywalkerToolsPool)

#pragma region SkywalkerPool Tests

bool TestPool_Init()
{
    SKYWALKER_POOL_NAMESPACE::CSkywalkerPool<int> Pool(5);
    SKYWALKER_TEST_ASSERT_EQ(0, (int)Pool.Size());
    return true;
}

bool TestPool_RecycleAndGet()
{
    SKYWALKER_POOL_NAMESPACE::CSkywalkerPool<int> Pool(5);
    Pool.Recycle(new int(100));
    SKYWALKER_TEST_ASSERT_EQ(1, (int)Pool.Size());
    
    int* obj = Pool.Get();
    SKYWALKER_TEST_ASSERT_NE(nullptr, obj);
    SKYWALKER_TEST_ASSERT_EQ(100, *obj);
    SKYWALKER_TEST_ASSERT_EQ(0, (int)Pool.Size());
    return true;
}

bool TestPool_MaxSize()
{
    SKYWALKER_POOL_NAMESPACE::CSkywalkerPool<int> Pool(2);
    Pool.Recycle(new int(1));
    Pool.Recycle(new int(2));
    Pool.Recycle(new int(3)); // Should be deleted
    SKYWALKER_TEST_ASSERT_EQ(2, (int)Pool.Size());
    return true;
}

#pragma endregion

SKYWALKER_TEST_SUITE(SkywalkerToolsSingleton)

#pragma region SkywalkerSingleton Tests

class TestSingletonClass
{
    SKYWALKER_SINGLETON_DECLARE(TestSingletonClass)
public:
    int Value = 0;
};

SKYWALKER_SINGLETON_IMPLEMENT(TestSingletonClass)

bool TestSingleton_GetInstance()
{
    TestSingletonClass::DestroyInstance();
    auto* Instance1 = TestSingletonClass::GetInstance();
    auto* Instance2 = TestSingletonClass::GetInstance();
    SKYWALKER_TEST_ASSERT_EQ(Instance1, Instance2);
    TestSingletonClass::DestroyInstance();
    return true;
}

bool TestSingleton_DestroyInstance()
{
    TestSingletonClass::GetInstance();
    TestSingletonClass::DestroyInstance();
    auto* Instance = TestSingletonClass::GetInstance();
    SKYWALKER_TEST_ASSERT_NE(nullptr, Instance);
    TestSingletonClass::DestroyInstance();
    return true;
}

#pragma endregion

// Register Tests - SkywalkerErrors
SKYWALKER_TEST_REGISTER(SkywalkerToolsErrors, TestErrors_Empty, TestErrors_Empty)
SKYWALKER_TEST_REGISTER(SkywalkerToolsErrors, TestErrors_AddError, TestErrors_AddError)
SKYWALKER_TEST_REGISTER(SkywalkerToolsErrors, TestErrors_GetFirstError, TestErrors_GetFirstError)
SKYWALKER_TEST_REGISTER(SkywalkerToolsErrors, TestErrors_GetErrors, TestErrors_GetErrors)
SKYWALKER_TEST_REGISTER(SkywalkerToolsErrors, TestErrors_ErrorTrace, TestErrors_ErrorTrace)

// Register Tests - SkywalkerPool
SKYWALKER_TEST_REGISTER(SkywalkerToolsPool, TestPool_Init, TestPool_Init)
SKYWALKER_TEST_REGISTER(SkywalkerToolsPool, TestPool_RecycleAndGet, TestPool_RecycleAndGet)
SKYWALKER_TEST_REGISTER(SkywalkerToolsPool, TestPool_MaxSize, TestPool_MaxSize)

// Register Tests - SkywalkerSingleton
SKYWALKER_TEST_REGISTER(SkywalkerToolsSingleton, TestSingleton_GetInstance, TestSingleton_GetInstance)
SKYWALKER_TEST_REGISTER(SkywalkerToolsSingleton, TestSingleton_DestroyInstance, TestSingleton_DestroyInstance)

#endif // __SKYWALKER_TOOLS_TEST_H__
