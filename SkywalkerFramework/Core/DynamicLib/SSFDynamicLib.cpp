/*************************************************************************
**文件: SkywalkerFramework\Core\DynamicLib\SSFDynamicLib.cpp
**作者: shyfan
**日期: 2023/08/12 16:07:58
**功能: 动态库
*************************************************************************/

#include "SSFDynamicLib.h"

#include <algorithm>
#include <cctype>

SF_NAMESPACE_USING

namespace
{
    bool SFEndsWithIgnoreCase(const SFString &Text, const SFString &Suffix)
    {
        if (Text.size() < Suffix.size())
        {
            return false;
        }

        const auto Offset = Text.size() - Suffix.size();
        for (size_t Index = 0; Index < Suffix.size(); ++Index)
        {
            const unsigned char Left = static_cast<unsigned char>(Text[Offset + Index]);
            const unsigned char Right = static_cast<unsigned char>(Suffix[Index]);
            if (std::tolower(Left) != std::tolower(Right))
            {
                return false;
            }
        }

        return true;
    }
}

/**
 * 构造函数
 * - 保存库名
 * - 初始化句柄为空
 * - 自动补全平台动态库扩展名（如 .dll / .so）
 */
SFDynamicLib::SFDynamicLib(const SFString &InName)
{
    Name = InName;
    Instance = nullptr;

    if (!SFEndsWithIgnoreCase(Name, SKYWALKER_DYNAMIC_LIB_EXT))
    {
        Name.append(SKYWALKER_DYNAMIC_LIB_EXT);
    }
}

/**
 * 析构函数
 * 说明：当前不在析构中强制卸载，调用方应在生命周期结束前显式执行 Unload。
 */
SFDynamicLib::~SFDynamicLib()
{
    Unload();
}

/**
 * 加载动态库
 * 过程：路径前缀 + 库名（含扩展名） -> 调用平台加载宏
 */
bool SFDynamicLib::Load()
{
    if (Instance != nullptr)
    {
        return true;
    }

    SFString Path = SKYWALKER__DYNAMIC_LIB_PATH;
    Path.append(Name);

    Instance = SKYWALKER_DYNAMIC_LIB_LOAD(Path.c_str());

    return Instance != nullptr;
}

/**
 * 卸载动态库
 * 说明：当前实现不校验句柄有效性与卸载返回值，统一返回 true。
 */
bool SFDynamicLib::Unload()
{
    if (Instance == nullptr)
    {
        return true;
    }

    const auto Result = SKYWALKER_DYNAMIC_LIB_UNLOAD(Instance);
    const bool bSuccess = SKYWALKER_DYNAMIC_LIB_UNLOAD_SUCCESS(Result);

    Instance = nullptr;
    return bSuccess;
}

const SFString &SFDynamicLib::GetName() const
{
    return Name;
}

/**
 * 获取导出符号地址
 * @param ProcName 导出函数名
 * @return 符号地址，失败时通常为 nullptr
 */
void *SFDynamicLib::GetSymbol(const char *ProcName)
{
    if (Instance == nullptr || ProcName == nullptr || ProcName[0] == '\0')
    {
        return nullptr;
    }

    return (void *)SKYWALKER_DYNAMIC_LIB_GET_SYMBOL(Instance, ProcName);
}
