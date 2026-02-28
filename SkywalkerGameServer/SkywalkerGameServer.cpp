/*************************************************************************
**文件: SkywalkerGameServer\Executable\SkywalkerGameServer.cpp
**作者: shyfan
**日期: 2023/08/03 11:19:47
**功能: 入口
*************************************************************************/

#include "Include/SFramework.h"
#include "SkywalkerPlatform/SkywalkerPlatform.h"

SSF_NAMESPACE_USING

int main(int argc, char *argv[])
{
    SSFString PluginConfigPath = "ServerPlugin.skywalkerC";
    SSFString ServerConfigPath = "ServerConfig.skywalkerC";

    if (argc > 1)
    {
        PluginConfigPath = argv[1];
    }
    if (argc > 2)
    {
        ServerConfigPath = argv[2];
    }

    SkywalkerSetEnv("SKYWALKER_PLUGIN_CONFIG", PluginConfigPath.c_str());
    SkywalkerSetEnv("SKYWALKER_SERVER_CONFIG", ServerConfigPath.c_str());

    SKYWALKER_FRAMEWORK_START(argc, argv)

    return 0;
}
