/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SSFTestExample.h
**作者: shyfan
**日期: 2026/02/26
**功能: 测试示例
*************************************************************************/

#ifndef __SKYWALKER_TEST_EXAMPLE_H__
#define __SKYWALKER_TEST_EXAMPLE_H__

#include "SkywalkerTest/SkywalkerTest.h"

SKYWALKER_TEST_SUITE(SkywalkerCore)

bool TestStringEmpty()
{
    std::string str;
    SKYWALKER_TEST_ASSERT_TRUE(str.empty());
    return true;
}

bool TestStringCompare()
{
    std::string a = "hello";
    std::string b = "hello";
    SKYWALKER_TEST_ASSERT_EQ(a, b);
    return true;
}

bool TestIntEqual()
{
    int a = 10;
    int b = 10;
    SKYWALKER_TEST_ASSERT_EQ(a, b);
    return true;
}

bool TestIntNotEqual()
{
    int a = 10;
    int b = 20;
    SKYWALKER_TEST_ASSERT_NE(a, b);
    return true;
}

bool TestVectorSize()
{
    std::vector<int> vec = {1, 2, 3, 4, 5};
    SKYWALKER_TEST_ASSERT_EQ((int)vec.size(), 5);
    return true;
}

SKYWALKER_TEST_REGISTER(SkywalkerCore, TestStringEmpty, TestStringEmpty)
SKYWALKER_TEST_REGISTER(SkywalkerCore, TestStringCompare, TestStringCompare)
SKYWALKER_TEST_REGISTER(SkywalkerCore, TestIntEqual, TestIntEqual)
SKYWALKER_TEST_REGISTER(SkywalkerCore, TestIntNotEqual, TestIntNotEqual)
SKYWALKER_TEST_REGISTER(SkywalkerCore, TestVectorSize, TestVectorSize)

#endif // __SKYWALKER_TEST_EXAMPLE_H__
