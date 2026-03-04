/*************************************************************************
**File: SkywalkerTools\SkywalkerPath\SkywalkerPath.h
**Author: shyfan
**Date: 2026/03/03
**Desc: Path Utility
*************************************************************************/

#ifndef __SKYWALKER_PATH_H__
#define __SKYWALKER_PATH_H__

#include <filesystem>
#include <string>

/**
 * 获取绝对路径
 * @param Path 输入路径（可为相对或绝对）
 * @return 归一化前缀后的绝对路径
 */
inline std::filesystem::path SkywalkerAbsolutePath(const std::filesystem::path &Path)
{
    return std::filesystem::absolute(Path);
}

/**
 * 解析配置路径：
 * 1) 绝对路径：直接返回；
 * 2) 带父目录的相对路径：相对 ExeDir；
 * 3) 纯文件名：相对 DefaultConfigDir。
 *
 * @param InPath 输入配置路径
 * @param ExeDir 可执行文件所在目录
 * @param DefaultConfigDir 默认配置目录
 * @return 可直接读取的规范化路径字符串
 */
inline std::string SkywalkerResolveConfigPath(const std::string &InPath,
                                              const std::filesystem::path &ExeDir,
                                              const std::filesystem::path &DefaultConfigDir)
{
    std::filesystem::path PathObj(InPath);
    if (PathObj.is_absolute())
    {
        return PathObj.lexically_normal().string();
    }

    if (PathObj.has_parent_path())
    {
        return (ExeDir / PathObj).lexically_normal().string();
    }

    return (DefaultConfigDir / PathObj).lexically_normal().string();
}

#endif // __SKYWALKER_PATH_H__
