/*************************************************************************
**文件: SkywalkerGameClient\Executable\SkywalkerGameClient.cpp
**作者: shyfan
**日期: 2023/08/07 11:34:09
**功能: 入口
*************************************************************************/

#include "Include/SFFramework.h"
#include "SkywalkerPlatform/SkywalkerPlatform.h"

SF_NAMESPACE_USING

int main(int argc, char *argv[])
{
    SFString PluginConfigPath = "ClientPlugin.skywalkerC";
    if (argc > 1)
    {
        PluginConfigPath = argv[1];
    }

    SkywalkerSetEnv("SKYWALKER_PLUGIN_CONFIG", PluginConfigPath.c_str());

    SKYWALKER_FRAMEWORK_START(argc, argv)

    return 0;
}
