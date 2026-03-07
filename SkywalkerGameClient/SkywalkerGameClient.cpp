/*************************************************************************
**文件: SkywalkerGameClient\Executable\SkywalkerGameClient.cpp
**作者: shyfan
**日期: 2023/08/07 11:34:09
**功能: 入口
*************************************************************************/

#include "Include/SFFramework.h"

SF_NAMESPACE_USING

int main(int argc, char *argv[])
{
    SFPath ExePath = SkywalkerAbsolutePath(SFPath(argv[0]));
    SFPath ExeDir = ExePath.parent_path();
    SFPath ConfigDir = (ExeDir / ".." / "Client").lexically_normal();
    SFString PluginConfigPath = "ClientPlugin.skywalkerC";
    SFString ClientConfigPath = "ClientConfig.skywalkerC";

    if (argc > 1)
    {
        PluginConfigPath = argv[1];
    }
    if (argc > 2)
    {
        ClientConfigPath = argv[2];
    }

    PluginConfigPath = SkywalkerResolveConfigPath(PluginConfigPath, ExeDir, ConfigDir);
    ClientConfigPath = SkywalkerResolveConfigPath(ClientConfigPath, ExeDir, ConfigDir);

    SkywalkerSetEnv(SF_ENV_PROGRAM_WORK_DIR, ExeDir.string().c_str());
    SkywalkerSetEnv(SF_ENV_PLUGIN_CONFIG_DIR, PluginConfigPath.c_str());
    SkywalkerSetEnv(SF_ENV_PROGRAM_CONFIG_DIR, ClientConfigPath.c_str());

    SKYWALKER_FRAMEWORK_START(argc, argv)

    return 0;
}
