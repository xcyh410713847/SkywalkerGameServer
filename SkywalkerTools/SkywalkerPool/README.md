# SkywalkerPool

## 功能
- 提供简单对象池，复用对象并减少频繁分配/释放。

## 主要类型
- `CSkywalkerPool<T>`
  - `Get()`：获取对象
  - `Recycle(T*)`：回收对象
  - `Size()`：当前池大小

## 使用示例
```cpp
auto Pool = SKYWALKER_POOL_NEW(MyObject, 128);
MyObject* Obj = Pool->Get();
Pool->Recycle(Obj);
```

## 注意事项
- `Get()` 前应保证池中有对象；否则 `front()` 行为未定义。
- 超过 `MaxSize` 回收会直接 `delete`。
- 当前实现非线程安全，多线程场景需加锁封装。
