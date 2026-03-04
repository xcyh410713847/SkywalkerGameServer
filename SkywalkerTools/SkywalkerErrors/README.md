# SkywalkerErrors

## 功能
- 提供通用错误收集容器与错误追踪信息封装。
- 适合在模块初始化、配置加载、业务流程中聚合错误。

## 主要类型
- `CSkywalkerErrors<T>`
  - `AddError(Error, Trace)`
  - `GetErrors()`
  - `GetErrorTrace()`
  - `GetFirstError()`
  - `IsValid()`

## 主要宏
- `SKYWALKER_ERRORS_WRAP(CppErrors, Error)`：仅记录错误。
- `SKYWALKER_ERRORS_WRAP_TRACE(CppErrors, Error)`：记录错误并附带文件/行号/函数。

## 使用示例
```cpp
CSkywalkerErrors<MyErrorCode> Errors;
SKYWALKER_ERRORS_WRAP_TRACE(Errors, MyErrorCode::ConfigLoadFailed);
if (Errors.IsValid())
{
    auto First = Errors.GetFirstError();
}
```

## 注意事项
- `GetErrors()` 和 `GetErrorTrace()` 返回拷贝；如有性能要求可按需扩展返回引用接口。
