# AGENTS.md - SkywalkerGameServer Development Guide

## Project Overview
C++20 game server framework with plugin-based architecture.

## Build Commands
```bash
cd build/SkywalkerGameServer
cmake ..
cmake --build . --config Debug
cmake --build . --target SkywalkerGameServer --config Debug
./Bin/Debug/SkywalkerGameServer.exe
```

### Build Modes
- `ProjectDebug` (default) - Debug builds
- `ProjectRelease` - Release builds
- `Library` - Library-only builds

### Output Locations
- Debug: `Bin/Debug/`
- Release: `Bin/Server/`

### Running Single Test
All tests run via `SkywalkerTestRunner::Instance().RunAll()`. No single-test filtering available - run all tests via:
```bash
cd build/SkywalkerGameServer
cmake .. && cmake --build . --target SkywalkerTestRunner --config Debug
./Bin/Debug/SkywalkerTestRunner.exe
```

### Linting
No linting tool configured. Code must compile and pass all tests before commit.

## Testing
Custom unit test framework at `SkywalkerTools/SkywalkerTest/`.

### Running Tests
```bash
cd build/SkywalkerGameServer
cmake .. && cmake --build . --target SkywalkerTestRunner --config Debug
./Bin/Debug/SkywalkerTestRunner.exe
```
All tests run via `SkywalkerTestRunner::Instance().RunAll()`. No single-test filtering.

### Writing Tests
```cpp
#include "SkywalkerTest/SkywalkerTest.h"
SKYWALKER_TEST_SUITE(MySuite)
bool TestMyFeature() { SKYWALKER_TEST_ASSERT_TRUE(1==1); return true; }
SKYWALKER_TEST_REGISTER(MySuite, TestMyFeature, TestMyFeature)
int main() { return SkywalkerTestRunner::Instance().RunAll(); }
```

### Test Macros
| Macro                                        | Description                     |
| -------------------------------------------- | ------------------------------- |
| `SKYWALKER_TEST_ASSERT(condition)`           | Assert condition is true        |
| `SKYWALKER_TEST_ASSERT_EQ(expected, actual)` | Assert two values are equal     |
| `SKYWALKER_TEST_ASSERT_NE(expected, actual)` | Assert two values are not equal |
| `SKYWALKER_TEST_ASSERT_TRUE(value)`          | Assert value is true            |
| `SKYWALKER_TEST_ASSERT_FALSE(value)`         | Assert value is false           |

## Code Style Guidelines

### Core Principles
- **All code must have unit tests** - No code without tests should be committed
- **Plugin-Module design** - Features as plugins with multiple modules
- **Cross-platform** - Design for Windows, Linux, iOS from the start

### File Organization
- Headers: `.h`, Implementation: `.cpp`
- Include guards: `#ifndef __FILE_PATH_H__`
- Include order: Framework core headers → local headers
- Use `#pragma region` for code organization

### Naming Conventions
| Type       | Convention          | Example                 |
| ---------- | ------------------- | ----------------------- |
| Classes    | `C` prefix          | `CSkywalkerFramework`   |
| Plugins    | `SSFPlugin_` prefix | `SSFPlugin_Test`        |
| Modules    | `SSFModule_` prefix | `SSFModule_TestOne`     |
| Interfaces | `I` prefix          | `ISSFPlugin`            |
| Enums      | `E` prefix          | `ESkywalkerServerState` |
| Files      | PascalCase          | `SkywalkerFramework.h`  |

### Type System (from SFCore.h)
- `SSFString` (std::string), `SSFBool` (bool), `SSFInt` (int)
- `SSFUInt` (unsigned int), `SSFFloat` (float), `SSFDouble` (double)

### Pointer Macros
`SSF_PTR(T), SSF_CONST_PTR(T), SSF_SHARED_PTR(T), SSF_WEAK_PTR(T), SSF_UNIQUE_PTR(T)`

### Namespace
`SF_NAMESPACE_BEGIN // code here SSF_NAMESPACE_END`

### Error Handling
```cpp
SSFObjectErrors Errors;
SSF_ERROR(Errors, ESFError::Plugin_Load_Failed);
SSF_ERROR_DESC(Errors, ESFError::Plugin_Load_Failed, "Description");
if (Errors.IsValid()) { /* handle error */ }
```

### Logging
`SF_LOG_INFO/ERROR/DEBUG("Message " << variable)`

### Assertions
`SSF_ASSERT(condition); SSF_ASSERT_IS_BASE_OF(SSFPlugin, MyPlugin);`

### Plugin/Module Development
```cpp
SSF_REGISTER_PLUGIN(PluginManager, SSFPlugin_MyPlugin);
SSF_UNREGISTER_PLUGIN(PluginManager, SSFPlugin_MyPlugin);
SF_PLUGIN_EXPORT(SSFPlugin_MyPlugin);
SSF_REGISTER_MODULE(SSFModule_MyModule);
SSF_UNREGISTER_MODULE(SSFModule_MyModule);
```

### File Header Format
```cpp
/*************************************************************************
**文件: SkywalkerFramework\Plugin\Test\SSFPlugin_Test.h
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
├── SkywalkerFramework/
│   ├── Core/              # Core library (static lib)
│   ├── Plugin/            # Dynamic plugins (AI, Actor, CommandLine, DB, Level, Network, Profiler, Test, Video)
│   └── Include/            # Framework headers
├── SkywalkerScript/       # Script encoding tool
└── SkywalkerTools/        # Utility libraries
```

## Cursor Rules
This project has `.cursorrules` with additional guidelines. All rules from `.cursorrules` apply and take precedence if conflicts arise.
