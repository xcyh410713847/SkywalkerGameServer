/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Session\SSFNetworkBlacklistStore.h
**作者: shyfan
**日期: 2026/03/02
**功能: 黑名单持久化存储
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_BLACKLIST_STORE_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_BLACKLIST_STORE_H__

#include "Include/SFCore.h"

#include <filesystem>

SF_NAMESPACE_BEGIN

struct SSFNetworkBlacklistSnapshot
{
    SFUInt32 FormatVersion = 1;
    SFMap<SFString, SFUInt64> Entries;
};

class SSFNetworkBlacklistStore
{
public:
    static bool LoadFromFile(const SFString &FilePath,
                             SFUInt64 NowMS,
                             SSFNetworkBlacklistSnapshot &OutSnapshot,
                             std::filesystem::file_time_type *OutLastWriteTime = nullptr);

    static bool SaveToFile(const SFString &FilePath,
                           const SSFNetworkBlacklistSnapshot &Snapshot,
                           std::filesystem::file_time_type *OutLastWriteTime = nullptr);
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_NETWORK_BLACKLIST_STORE_H__
