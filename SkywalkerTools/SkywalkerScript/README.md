# SkywalkerScript

## 功能
- 提供配置文件解析器，支持层级结构的 `key=value` 格式。
- 支持注释（#）、空行跳过、缩进层级解析。

## 主要类型
- `CSkywalkerScriptNode` - 脚本节点
  - `GetNodeName()` - 获取节点名称
  - `GetChildNodeFromName()` - 通过名称获取子节点
  - `GetChildNodeFromIndex()` - 通过索引获取子节点
  - `GetNodeValueInt()` / `GetNodeValueFloat()` / `GetNodeValueString()` / `GetNodeValueBool()` - 获取节点值
- `CSkywalkerScriptParse` - 解析器
  - `LoadScript()` - 加载脚本文件
  - `GetRootNode()` - 获取根节点

## 命名空间
- `Skywalker::Script`

## 关键宏
- `SKYWALKER_PTR_SCRIPT_PARSE` - 解析器指针
- `SKYWALKER_PTR_SCRIPT_NODE` - 节点指针

## 使用示例
```cpp
#include "SkywalkerTools/SkywalkerScript/SkywalkerScript.h"

SKYWALKER_SCRIPT_NAMESPACE_USING;

CSkywalkerScriptParse* Parser = new CSkywalkerScriptParse();
if (Parser->LoadScript("config.skywalkerC"))
{
    SKYWALKER_PTR_SCRIPT_NODE Root = Parser->GetRootNode();
    SKYWALKER_PTR_SCRIPT_NODE Child = Root->GetChildNodeFromName("Server");
    if (Child)
    {
        const char* IP = Child->GetChildNodeFromName("IP")->GetNodeValueString();
        int Port = Child->GetChildNodeFromName("Port")->GetNodeValueInt();
    }
}
```

## 注意事项
- 当前实现非线程安全，多线程场景需加锁封装。
- 节点内存由解析器管理，解析器销毁时自动释放。
