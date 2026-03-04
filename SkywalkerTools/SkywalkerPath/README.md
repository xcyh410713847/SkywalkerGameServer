# SkywalkerPath

## 功能
- 提供与业务无关的路径处理工具，便于跨项目复用。
- 统一入口程序的配置路径解析规则。

## 主要接口
- `SkywalkerAbsolutePath(const std::filesystem::path&)`
- `SkywalkerResolveConfigPath(const std::string&, const std::filesystem::path&, const std::filesystem::path&)`

## 解析规则
`SkywalkerResolveConfigPath` 规则：
1. 绝对路径：直接返回。
2. 带父目录的相对路径：相对 `ExeDir`。
3. 纯文件名：相对 `DefaultConfigDir`。

## 使用示例
```cpp
auto ExeDir = SkywalkerAbsolutePath(argv[0]).parent_path();
auto ConfigDir = (ExeDir / ".." / "Server").lexically_normal();
auto ServerConfigPath = SkywalkerResolveConfigPath("ServerConfig.skywalkerC", ExeDir, ConfigDir);
```

## 注意事项
- 函数仅做路径拼接与规范化，不检查文件是否存在。
