# AGENTS.md - SkywalkerGameServer Development Guide

## Project Overview

C++20 game server framework with plugin-based architecture.

## Build Commands

```bash
# Configure and build
cd build/SkywalkerGameServer
cmake ..
cmake --build . --config Debug

# Build specific target
cmake --build . --target SkywalkerGameServer --config Debug

# Run server
./Bin/Debug/SkywalkerGameServer.exe
```

### Build Modes

- `ProjectDebug` (default) - Debug builds
- `ProjectRelease` - Release builds
- `Library` - Library-only builds

### Output Locations

- Debug: `Bin/Debug/`
- Release: `Bin/Server/`

## Testing

Custom unit test framework at `SkywalkerTools/SkywalkerTest/`.

### Running Tests

```cpp
#include "SkywalkerTest/SkywalkerTest.h"

int main()
{
    return SkywalkerTestRunner::Instance().RunAll();
}
```

### Writing Tests

```cpp
#include "SkywalkerTest/SkywalkerTest.h"

SKYWALKER_TEST_SUITE(MySuite)

bool TestMyFeature()
{
    SKYWALKER_TEST_ASSERT_TRUE(someCondition);
    SKYWALKER_TEST_ASSERT_EQ(expected, actual);
    return true;
}

SKYWALKER_TEST_REGISTER(MySuite, TestMyFeature, TestMyFeature)
```

### Test Macros

| Macro | Description |
|-------|-------------|
| `SKYWALKER_TEST_ASSERT(condition)` | Assert condition is true |
| `SKYWALKER_TEST_ASSERT_EQ(expected, actual)` | Assert two values are equal |
| `SKYWALKER_TEST_ASSERT_NE(expected, actual)` | Assert two values are not equal |
| `SKYWALKER_TEST_ASSERT_TRUE(value)` | Assert value is true |
| `SKYWALKER_TEST_ASSERT_FALSE(value)` | Assert value is false |

## Code Style Guidelines

### Core Principles

- **All code must have unit tests** - No code without tests should be committed
- **Plugin-Module design** - Features as plugins with multiple modules
- **Cross-platform** - Design for Windows, Linux, iOS from the start

### File Organization

- Headers: `.h`, Implementation: `.cpp`
- Include guards: `#ifndef __FILE_PATH_H__`
- Include order: Framework core headers → local headers

### Naming Conventions

| Type | Convention | Example |
|------|------------|---------|
| Classes | `C` prefix | `CSkywalkerServerFramework` |
| Plugins | `SSFPlugin_` prefix | `SSFPlugin_Test` |
| Modules | `SSFModule_` prefix | `SSFModule_TestOne` |
| Interfaces | `I` prefix | `ISSFPlugin` |
| Enums | `E` prefix | `ESkywalkerServerState` |
| Files | PascalCase | `SkywalkerServerFramework.h` |

### Type System (from SSFCore.h)

- `SSFString` (std::string), `SSFBool` (bool), `SSFInt` (int)
- `SSFUInt` (unsigned int), `SSFFloat` (float), `SSFDouble` (double)

### Pointer Macros

```cpp
SSF_PTR(T)           // Raw pointer T*
SSF_CONST_PTR(T)     // Const pointer const T*
SSF_SHARED_PTR(T)    // shared_ptr<T>
SSF_WEAK_PTR(T)      // weak_ptr<T>
SSF_UNIQUE_PTR(T)    // unique_ptr<T>
```

### Namespace

```cpp
SSF_NAMESPACE_BEGIN
// code here
SSF_NAMESPACE_END
```

### Error Handling

```cpp
SSFObjectErrors Errors;
SSF_ERROR(Errors, SkywalkerSFError_Plugin_Load_Failed);
SSF_ERROR_DESC(Errors, SkywalkerSFError_Plugin_Load_Failed, "Description");
if (Errors.IsValid()) { /* handle error */ }
```

### Logging

```cpp
SSF_LOG_INFO("Message " << variable);
SSF_LOG_ERROR("Message " << variable);
SSF_LOG_DEBUG("Message " << variable);
```

### Assertions

```cpp
SSF_ASSERT(condition);
SSF_ASSERT_IS_BASE_OF(SSFPlugin, MyPlugin);
```

### Plugin/Module Development

```cpp
SSF_REGISTER_PLUGIN(PluginManager, SSFPlugin_MyPlugin);
SSF_UNREGISTER_PLUGIN(PluginManager, SSFPlugin_MyPlugin);
SSF_PLUGIN_EXPORT(SSFPlugin_MyPlugin);  // DLL export

SSF_REGISTER_MODULE(SSFModule_MyModule);
SSF_UNREGISTER_MODULE(SSFModule_MyModule);
```

### Code Regions

```cpp
#pragma region SSFPlugin
// related code
#pragma endregion SSFPlugin
```

### File Header

```cpp
/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\SSFPlugin_Test.h
**作者: shyfan
**日期: 2023/08/21 19:54:39
**功能: 测试插件
*************************************************************************/
```

### Comments

- Use Chinese comments (project convention)
- Format: `/** 注释内容 */` or `// 注释内容`
- Document all public methods

## Common Issues

1. **Macro errors**: Check for single-line comments → use multi-line comments
2. **Enum undeclared**: Check if previous line has Chinese comment → try changing encoding
3. **_CrtIsValidHeapPointer**: Incorrectly released a smart pointer

## Project Structure

```
SkywalkerGameServer/
├── Bin/                    # Executable output
├── build/                  # CMake build files
├── SkywalkerGameServer/    # Main server executable
├── SkywalkerGameClient/   # Test client
├── SkywalkerServerFramework/
│   ├── Core/              # Core library (static lib)
│   ├── Plugin/            # Dynamic plugins
│   │   ├── AI/ Actor/ CommandLine/
│   │   ├── DB/ Level/ Network/
│   │   ├── Profiler/ Test/ Video/
│   └── Include/            # Framework headers
├── SkywalkerScript/       # Script encoding tool
└── SkywalkerTools/        # Utility libraries
```

## Cursor Rules

This project has `.cursorrules` with additional guidelines. All rules from `.cursorrules` apply and take precedence if conflicts arise.
