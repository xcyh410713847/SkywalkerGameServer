# SkywalkerTest

## 功能
- 提供轻量测试宏与测试运行器。
- 用于验证 `SkywalkerTools` 与框架核心模块的基础行为。

## 主要内容
- 测试套件宏：`SKYWALKER_TEST_SUITE`
- 注册宏：`SKYWALKER_TEST_REGISTER`
- 断言宏：
  - `SKYWALKER_TEST_ASSERT`
  - `SKYWALKER_TEST_ASSERT_EQ`
  - `SKYWALKER_TEST_ASSERT_NE`
  - `SKYWALKER_TEST_ASSERT_TRUE`
  - `SKYWALKER_TEST_ASSERT_FALSE`

## 最小示例
```cpp
SKYWALKER_TEST_SUITE(MySuite)

bool TestSample()
{
    SKYWALKER_TEST_ASSERT_TRUE(1 == 1);
    return true;
}

SKYWALKER_TEST_REGISTER(MySuite, TestSample, TestSample)
```

## 注意事项
- 当前项目测试统一由 `SkywalkerTestRunner::Instance().RunAll()` 执行。
