# SkywalkerDerived

## 功能
- 提供编译期继承关系检测工具。
- 用于模板约束、宏约束、静态检查。

## 主要接口
- `SKYWALKER_IS_DERIVED(DerivedType, BaseType)`
  - 返回 `1/0`，表示 `DerivedType` 是否继承自 `BaseType`。

## 使用示例
```cpp
static_assert(SKYWALKER_IS_DERIVED(MyDerived, MyBase), "MyDerived must derive from MyBase");
```

## 依赖
- C++ 标准语言特性（模板/重载解析）。

## 注意事项
- 该工具是编译期检查，不产生运行时开销。
