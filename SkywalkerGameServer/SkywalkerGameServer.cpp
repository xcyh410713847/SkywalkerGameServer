/*************************************************************************
**文件: SkywalkerGameServer\Executable\SkywalkerGameServer.cpp
**作者: shyfan
**日期: 2023/08/03 11:19:47
**功能: 入口
*************************************************************************/

#include "Include/SFFramework.h"
#include "SkywalkerPlatform/SkywalkerPlatform.h"

#include <filesystem>

SF_NAMESPACE_USING

int main(int argc, char *argv[])
{
    SFString ConfigDir = "../Server";
    SFString PluginConfigPath = "ServerPlugin.skywalkerC";
    SFString ServerConfigPath = "ServerConfig.skywalkerC";

    auto ResolveConfigPath = [&ConfigDir](const SFString &InPath) -> SFString
    {
        std::filesystem::path PathObj(InPath);
        if (PathObj.is_absolute())
        {
            return PathObj.lexically_normal().string();
        }

        if (PathObj.has_parent_path())
        {
            return PathObj.lexically_normal().string();
        }

        return (std::filesystem::path(ConfigDir) / PathObj).lexically_normal().string();
    };

    if (argc > 1)
    {
        PluginConfigPath = argv[1];
    }
    if (argc > 2)
    {
        ServerConfigPath = argv[2];
    }

    PluginConfigPath = ResolveConfigPath(PluginConfigPath);
    ServerConfigPath = ResolveConfigPath(ServerConfigPath);

    SkywalkerSetEnv("SKYWALKER_PLUGIN_CONFIG", PluginConfigPath.c_str());
    SkywalkerSetEnv("SKYWALKER_SERVER_CONFIG", ServerConfigPath.c_str());

    SKYWALKER_FRAMEWORK_START(argc, argv)

    return 0;
}
