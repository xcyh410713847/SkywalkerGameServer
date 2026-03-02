/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Session\SSFNetworkBlacklistStore.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 黑名单持久化存储
*************************************************************************/

#include "SSFNetworkBlacklistStore.h"

#include <fstream>
#include <sstream>

SF_NAMESPACE_USING

bool SSFNetworkBlacklistStore::LoadFromFile(const SFString &FilePath,
                                            SFUInt64 NowMS,
                                            SSFNetworkBlacklistSnapshot &OutSnapshot,
                                            std::filesystem::file_time_type *OutLastWriteTime)
{
    OutSnapshot.Entries.clear();

    std::ifstream InFile(FilePath);
    if (!InFile.is_open())
    {
        return false;
    }

    SFString Line;
    while (std::getline(InFile, Line))
    {
        if (Line.empty())
        {
            continue;
        }

        if (Line[0] == '#')
        {
            if (Line.rfind("#Version", 0) == 0)
            {
                std::istringstream VersionStream(Line.substr(8));
                SFUInt32 Version = 1;
                VersionStream >> Version;
                OutSnapshot.FormatVersion = Version;
            }
            continue;
        }

        std::istringstream Stream(Line);
        SFString IP;
        SFUInt64 UntilMS = 0;
        Stream >> IP >> UntilMS;
        if (IP.empty() || UntilMS == 0)
        {
            continue;
        }

        if (UntilMS > NowMS)
        {
            OutSnapshot.Entries[IP] = UntilMS;
        }
    }

    if (OutLastWriteTime != nullptr && std::filesystem::exists(FilePath))
    {
        *OutLastWriteTime = std::filesystem::last_write_time(FilePath);
    }

    return true;
}

bool SSFNetworkBlacklistStore::SaveToFile(const SFString &FilePath,
                                          const SSFNetworkBlacklistSnapshot &Snapshot,
                                          std::filesystem::file_time_type *OutLastWriteTime)
{
    std::filesystem::path PathObj(FilePath);
    if (PathObj.has_parent_path())
    {
        std::filesystem::create_directories(PathObj.parent_path());
    }

    std::ofstream OutFile(FilePath, std::ios::trunc);
    if (!OutFile.is_open())
    {
        return false;
    }

    OutFile << "#Version " << Snapshot.FormatVersion << "\n";
    SF_COMMON_ITERATOR(Iter, Snapshot.Entries)
    {
        OutFile << Iter->first << " " << Iter->second << "\n";
    }

    OutFile.flush();

    if (OutLastWriteTime != nullptr && std::filesystem::exists(FilePath))
    {
        *OutLastWriteTime = std::filesystem::last_write_time(FilePath);
    }

    return true;
}
