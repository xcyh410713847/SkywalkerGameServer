# SkywalkerSingleton

## 功能
- 提供单例模式声明/实现宏，减少样板代码。

## 主要宏
- `SKYWALKER_SINGLETON_DECLARE(Class)`：写在类定义中。
- `SKYWALKER_SINGLETON_IMPLEMENT(Class)`：写在一个 `.cpp` 中。

## 使用示例
```cpp
class CConfigCenter
{
    SKYWALKER_SINGLETON_DECLARE(CConfigCenter)
public:
    void Load();
};

SKYWALKER_SINGLETON_IMPLEMENT(CConfigCenter)
```

## 注意事项
- 当前实现不是线程安全懒加载。
- 若需跨线程安全，建议改为 `std::call_once` 或局部静态对象方案。
