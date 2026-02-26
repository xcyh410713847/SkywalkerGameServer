# AGENTS.md - SkywalkerGameServer Development Guide

## Project Overview

SkywalkerGameServer is a C++20 game server framework with a plugin-based architecture. It uses CMake for building and produces both static libraries and dynamic plugins.

## Build Commands

### Building the Project

```bash
# Navigate to build directory and configure
cd build/SkywalkerGameServer
cmake ..

# Build all targets
cmake --build . --config Debug

# Or build a specific target
cmake --build . --target SkywalkerGameServer --config Debug
```

### Build Modes

The project supports three build modes (set via CMake cache variable `skywalker_server_MODE`):
- `ProjectDebug` (default) - Debug builds
- `ProjectReleas` - Release builds  
- `Library` - Library-only builds

### Output Locations

- Debug: `Bin/Debug/`
- Release: `Bin/Server/`

### Running the Server

```bash
./Bin/Debug/SkywalkerGameServer.exe
```

### Testing

The project uses a custom unit testing framework located in `SkywalkerTools/SkywalkerTest/`.

#### Writing Tests

```cpp
#include "SkywalkerTest/SkywalkerTest.h"

// Define a test suite
SKYWALKER_TEST_SUITE(MySuite)

// Write test functions
bool TestMyFeature()
{
    SKYWALKER_TEST_ASSERT_TRUE(someCondition);
    SKYWALKER_TEST_ASSERT_EQ(expected, actual);
    SKYWALKER_TEST_ASSERT_NE(expected, actual);
    return true;
}

// Register tests (auto-registers at startup)
SKYWALKER_TEST_REGISTER(MySuite, TestMyFeature, TestMyFeature)
```

#### Running Tests

```cpp
#include "SkywalkerTest/SkywalkerTest.h"

int main()
{
    return SkywalkerTestRunner::Instance().RunAll();
}
```

#### Test Macros

| Macro | Description |
|-------|-------------|
| `SKYWALKER_TEST_ASSERT(condition)` | Assert condition is true |
| `SKYWALKER_TEST_ASSERT_EQ(expected, actual)` | Assert two values are equal |
| `SKYWALKER_TEST_ASSERT_NE(expected, actual)` | Assert two values are not equal |
| `SKYWALKER_TEST_ASSERT_TRUE(value)` | Assert value is true |
| `SKYWALKER_TEST_ASSERT_FALSE(value)` | Assert value is false |

## Code Style Guidelines

### Fundamental Principle

**All code must have unit tests** - No code should be committed without corresponding unit tests.

### Architecture Principle

**Use Plugin-Module design** - All features should be implemented as plugins, and each plugin can contain multiple modules. This ensures:
- Loose coupling between features
- Easy to enable/disable functionality
- Clear separation of concerns

### Platform Principle

**All code must be cross-platform** - Design for Windows, Linux, and iOS from the start. Use platform abstraction layers and avoid platform-specific code directly in core modules.

### File Organization

- Header files: `.h` extension
- Implementation files: `.cpp` extension
- Use include guards: `#ifndef __FILE_PATH_H__`
- Order includes: Framework core headers first, then local headers
- Example:
  ```cpp
  #include "Include/SSFCore.h"
  #include "Include/SSFramework.h"
  #include "Core/Object/SSFObject.h"
  ```

### Naming Conventions

- **Classes**: Prefix with `C` (e.g., `CSkywalkerServerFramework`)
- **Plugins**: Prefix with `SSFPlugin_` (e.g., `SSFPlugin_Test`)
- **Modules**: Prefix with `SSFModule_` (e.g., `SSFModule_TestOne`)
- **Interfaces/Abstract**: Prefix with `I` (e.g., `ISSFPlugin`)
- **Enums**: Prefix with `E` (e.g., `ESkywalkerServerFrameworkRunningState`)
- **Files**: Use PascalCase (e.g., `SkywalkerServerFramework.h`)

### Namespace Usage

Always use the SSF namespace macros:
```cpp
SSF_NAMESPACE_BEGIN

// code here

SSF_NAMESPACE_END
```

### Type System

Use custom framework types defined in `SSFCore.h`:
- `SSFString` - std::string
- `SSFBool` - bool
- `SSFInt` - int
- `SSFUInt` - unsigned int
- `SSFFloat` - float
- `SSFDouble` - double

### Pointer Conventions

Use framework pointer macros from `SSFCore.h`:
- Raw pointer: `SSF_PTR(T)`
- Const pointer: `SSF_CONST_PTR(T)`
- Shared pointer: `SSF_SHARED_PTR(T)`
- Weak pointer: `SSF_WEAK_PTR(T)`
- Unique pointer: `SSF_UNIQUE_PTR(T)`

Example:
```cpp
SSF_PTR(SSFPluginManager) PluginManager{};
SSF_SHARED_PTR(SSFObject) Object = SSF_MAKE_SHARED_PTR(SSFObject);
```

### Code Regions

Use `#pragma region` for organizing code:
```cpp
#pragma region SSFPlugin

// related code

#pragma endregion SSFPlugin
```

### Error Handling

Use the custom error system:
```cpp
SSFObjectErrors Errors;

// Add error without stack trace
SSF_ERROR(Errors, SkywalkerSFError_Plugin_Load_Failed);

// Add error with description
SSF_ERROR_DESC(Errors, SkywalkerSFError_Plugin_Load_Failed, "Custom description");

// Check for errors
if (Errors.IsValid())
{
    auto FirstError = Errors.GetFirstError();
    // handle error
}
```

### Logging

Use the logging macros:
```cpp
SSF_LOG_INFO("Message " << variable);
SSF_LOG_ERROR("Message " << variable);
SSF_LOG_DEBUG("Message " << variable);
```

### Assertions

Use framework assertions:
```cpp
SSF_ASSERT(condition);
SSF_ASSERT_IS_BASE_OF(SSFPlugin, MyPlugin);
```

### Plugin Development

Use the plugin registration macros:
```cpp
// Register a plugin
SSF_REGISTER_PLUGIN(PluginManager, SSFPlugin_MyPlugin);

// Unregister a plugin
SSF_UNREGISTER_PLUGIN(PluginManager, SSFPlugin_MyPlugin);

// Plugin export macro (for DLL)
SSF_PLUGIN_EXPORT(SSFPlugin_MyPlugin);
```

### Module Development

Use the module registration macros:
```cpp
// Register a module
SSF_REGISTER_MODULE(SSFModule_MyModule);

// Unregister a module
SSF_UNREGISTER_MODULE(SSFModule_MyModule);
```

### Code Header Format

Each source file should have a header comment:
```cpp
/*************************************************************************
**文件: SkywalkerServerFramework\Plugin\Test\SSFPlugin_Test.h
**作者: shyfan
**日期: 2023/08/21 19:54:39
**功能: 测试插件
*************************************************************************/
```

### Comments

- Use Chinese comments (as per project convention)
- Comment format: `/** 注释内容 */` or `// 注释内容`
- Document all public methods with comments

### Common Issues (FAQ)

1. **Macro errors**: Check for single-line comments in macros - use multi-line comments
2. **Enum undeclared**: Check if previous line has Chinese comment - try changing encoding
3. **_CrtlsValidHeapPointer**: Likely released a smart pointer incorrectly

## Project Structure

```
SkywalkerGameServer/
├── Bin/                    # Executable output
├── build/                  # CMake build files
├── SkywalkerGameServer/    # Main server executable
├── SkywalkerGameClient/    # Test client
├── SkywalkerServerEditor/ # Server editor
├── SkywalkerServerFramework/
│   ├── Core/              # Core library (static lib)
│   ├── Plugin/            # Dynamic plugins
│   │   ├── AI/
│   │   ├── Actor/
│   │   ├── CommandLine/
│   │   ├── DB/
│   │   ├── Level/
│   │   ├── Network/
│   │   ├── Profiler/
│   │   ├── Test/
│   │   └── Video/
│   └── Include/            # Framework headers
├── SkywalkerScript/       # Script encoding tool
└── SkywalkerTools/        # Utility libraries
```

## IDE

The project includes a Visual Studio Code workspace file (`SkywalkerGameServer.code-workspace`). Configure C++20 support in your IDE.
