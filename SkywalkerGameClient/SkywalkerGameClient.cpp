/*************************************************************************
**文件: SkywalkerGameClient\Executable\SkywalkerGameClient.cpp
**作者: shyfan
**日期: 2023/08/07 11:34:09
**功能: 入口
*************************************************************************/

#include "Include/SFFramework.h"
#include "SkywalkerPlatform/SkywalkerPlatform.h"

#include <filesystem>

SF_NAMESPACE_USING

int main(int argc, char *argv[])
{
    std::filesystem::path ExePath = std::filesystem::absolute(std::filesystem::path(argv[0]));
    std::filesystem::path ExeDir = ExePath.parent_path();
    SFString ConfigDir = (ExeDir / ".." / "Client").lexically_normal().string();
    SFString PluginConfigPath = "ClientPlugin.skywalkerC";
    SFString ClientConfigPath = "ClientConfig.skywalkerC";

    auto ResolveConfigPath = [&ConfigDir, &ExeDir](const SFString &InPath) -> SFString
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

        return (std::filesystem::path(ConfigDir) / PathObj).lexically_normal().string();
    };

    if (argc > 1)
    {
        PluginConfigPath = argv[1];
    }
    if (argc > 2)
    {
        ClientConfigPath = argv[2];
    }

    PluginConfigPath = ResolveConfigPath(PluginConfigPath);
    ClientConfigPath = ResolveConfigPath(ClientConfigPath);

    SkywalkerSetEnv("SKYWALKER_PLUGIN_CONFIG", PluginConfigPath.c_str());
    SkywalkerSetEnv("SKYWALKER_CLIENT_CONFIG", ClientConfigPath.c_str());

    SKYWALKER_FRAMEWORK_START(argc, argv)

    return 0;
}
