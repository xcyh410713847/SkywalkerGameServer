/*************************************************************************
**文件: SkywalkerTools\SkywalkerTest\SkywalkerTest.h
**作者: shyfan
**日期: 2026/02/26
**功能: 单元测试框架
*************************************************************************/

#ifndef __SKYWALKER_TEST_H__
#define __SKYWALKER_TEST_H__

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>

#define SKYWALKER_TEST_ASSERT(condition) \
    if (!(condition)) { return false; }

#define SKYWALKER_TEST_ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { return false; }

#define SKYWALKER_TEST_ASSERT_NE(expected, actual) \
    if ((expected) == (actual)) { return false; }

#define SKYWALKER_TEST_ASSERT_TRUE(value) \
    SKYWALKER_TEST_ASSERT(value == true)

#define SKYWALKER_TEST_ASSERT_FALSE(value) \
    SKYWALKER_TEST_ASSERT(value == false)

class SkywalkerTestCase
{
public:
    using TestFunc = std::function<bool()>;

    SkywalkerTestCase(const std::string &InName, TestFunc InFunc)
        : Name(InName), Func(InFunc), Passed(false), ErrorMessage("")
    {
    }

    bool Run()
    {
        try
        {
            Passed = Func();
            if (!Passed)
            {
                ErrorMessage = "Test assertion failed";
            }
        }
        catch (const std::exception &e)
        {
            Passed = false;
            ErrorMessage = e.what();
        }
        catch (...)
        {
            Passed = false;
            ErrorMessage = "Unknown exception";
        }
        return Passed;
    }

    std::string Name;
    TestFunc Func;
    bool Passed;
    std::string ErrorMessage;
};

class SkywalkerTestSuite
{
public:
    void AddTest(const std::string &Name, SkywalkerTestCase::TestFunc Func)
    {
        Tests.emplace_back(Name, Func);
    }

    int RunAll()
    {
        int PassedCount = 0;
        int FailedCount = 0;

        std::cout << "========================================" << std::endl;
        std::cout << "Running " << Tests.size() << " tests..." << std::endl;
        std::cout << "========================================" << std::endl;

        for (auto &Test : Tests)
        {
            bool Result = Test.Run();
            if (Result)
            {
                std::cout << "[PASS] " << Test.Name << std::endl;
                PassedCount++;
            }
            else
            {
                std::cout << "[FAIL] " << Test.Name;
                if (!Test.ErrorMessage.empty())
                {
                    std::cout << " - " << Test.ErrorMessage;
                }
                std::cout << std::endl;
                FailedCount++;
            }
        }

        std::cout << "========================================" << std::endl;
        std::cout << "Results: " << PassedCount << " passed, " << FailedCount << " failed" << std::endl;
        std::cout << "========================================" << std::endl;

        return FailedCount;
    }

    int GetTestCount() const { return (int)Tests.size(); }

    void Clear() { Tests.clear(); }

private:
    std::vector<SkywalkerTestCase> Tests;
};

class SkywalkerTestRunner
{
public:
    static SkywalkerTestRunner &Instance()
    {
        static SkywalkerTestRunner Instance;
        return Instance;
    }

    void RegisterSuite(const std::string &SuiteName, SkywalkerTestSuite *Suite)
    {
        Suites[SuiteName] = Suite;
    }

    int RunAll()
    {
        int TotalFailed = 0;

        std::cout << "========================================" << std::endl;
        std::cout << "        Skywalker Test Runner          " << std::endl;
        std::cout << "========================================" << std::endl;

        for (auto &[Name, Suite] : Suites)
        {
            std::cout << "\nRunning Test Suite: " << Name << std::endl;
            int Failed = Suite->RunAll();
            TotalFailed += Failed;
        }

        std::cout << "\n========================================" << std::endl;
        std::cout << "Total: " << TotalFailed << " test(s) failed" << std::endl;
        std::cout << "========================================" << std::endl;

        return TotalFailed;
    }

    void Clear() { Suites.clear(); }

private:
    std::map<std::string, SkywalkerTestSuite *> Suites;
};

#define SKYWALKER_TEST_SUITE(Name) \
    static SkywalkerTestSuite *Get##Name##Suite() \
    { \
        static SkywalkerTestSuite Suite; \
        return &Suite; \
    }

#define SKYWALKER_TEST_REGISTER(SuiteName, TestName, Func) \
    static bool TestName##_func() { return Func(); } \
    static SkywalkerTestCase TestName##_case(#TestName, TestName##_func); \
    static struct TestName##_registrar { \
        TestName##_registrar() { \
            SkywalkerTestRunner::Instance().RegisterSuite(#SuiteName, Get##SuiteName##Suite()); \
            Get##SuiteName##Suite()->AddTest(#TestName, TestName##_func); \
        } \
    } TestName##_registrar_instance;

#endif // __SKYWALKER_TEST_H__
