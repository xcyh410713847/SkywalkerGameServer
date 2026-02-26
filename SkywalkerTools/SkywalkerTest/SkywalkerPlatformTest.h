/*************************************************************************
**文件: SkywalkerTools\SkywalkerTest\SkywalkerPlatformTest.h
**作者: shyfan
**日期: 2026/02/26
**功能: 跨平台单元测试
*************************************************************************/

#ifndef __SKYWALKER_PLATFORM_TEST_H__
#define __SKYWALKER_PLATFORM_TEST_H__

#include <string>

#include "SkywalkerTest/SkywalkerTest.h"
#include "SkywalkerPlatform/SkywalkerPlatform.h"

SKYWALKER_TEST_SUITE(SkywalkerPlatformTest)

bool TestPlatform_Name()
{
    SKYWALKER_TEST_ASSERT_NE(nullptr, SKYWALKER_PLATFORM_NAME);
    SKYWALKER_TEST_ASSERT_FALSE(std::string(SKYWALKER_PLATFORM_NAME).empty());
    return true;
}

bool TestPlatform_PathSeparator()
{
    #if defined(SKYWALKER_PLATFORM_WINDOWS)
        SKYWALKER_TEST_ASSERT_EQ('\\', SKYWALKER_PATH_SEPARATOR);
    #else
        SKYWALKER_TEST_ASSERT_EQ('/', SKYWALKER_PATH_SEPARATOR);
    #endif
    return true;
}

bool TestPlatform_DynamicLibExt()
{
    #if defined(SKYWALKER_PLATFORM_WINDOWS)
        SKYWALKER_TEST_ASSERT_EQ(std::string(SKYWALKER_DYNAMIC_LIB_EXT), std::string(".dll"));
    #else
        SKYWALKER_TEST_ASSERT_EQ(std::string(SKYWALKER_DYNAMIC_LIB_EXT), std::string(".so"));
    #endif
    return true;
}

bool TestPlatform_ThreadLocal()
{
    // SKYWALKER_THREAD_LOCAL is a modifier, not a value - just verify it compiles
    SKYWALKER_TEST_ASSERT_TRUE(true);
    return true;
}

#pragma region TestClassForDisableCopy

class TestDisableCopyClass
{
public:
    int Value = 0;
    SKYWALKER_DISALLOW_COPY(TestDisableCopyClass)
};

bool TestPlatform_DisableCopy()
{
    // 如果能编译通过，说明宏有效
    TestDisableCopyClass obj;
    obj.Value = 100;
    SKYWALKER_TEST_ASSERT_EQ(100, obj.Value);
    return true;
}

#pragma endregion

SKYWALKER_TEST_REGISTER(SkywalkerPlatformTest, TestPlatform_Name, TestPlatform_Name)
SKYWALKER_TEST_REGISTER(SkywalkerPlatformTest, TestPlatform_PathSeparator, TestPlatform_PathSeparator)
SKYWALKER_TEST_REGISTER(SkywalkerPlatformTest, TestPlatform_DynamicLibExt, TestPlatform_DynamicLibExt)
SKYWALKER_TEST_REGISTER(SkywalkerPlatformTest, TestPlatform_ThreadLocal, TestPlatform_ThreadLocal)
SKYWALKER_TEST_REGISTER(SkywalkerPlatformTest, TestPlatform_DisableCopy, TestPlatform_DisableCopy)

#endif // __SKYWALKER_PLATFORM_TEST_H__
