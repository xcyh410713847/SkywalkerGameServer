/*************************************************************************
**文件: SkywalkerGameServer\Executable\SkywalkerGameServer.cpp
**作者: shyfan
**日期: 2023/08/03 11:19:47
**功能: 入口
*************************************************************************/

#include "Include/SFFramework.h"
#include "SkywalkerPath/SkywalkerPath.h"
#include "SkywalkerPlatform/SkywalkerPlatform.h"

SF_NAMESPACE_USING

int main(int argc, char *argv[])
{
    /** 计算程序所在目录，并构造默认配置目录（Bin/Server） */
    SFPath ExePath = SkywalkerAbsolutePath(SFPath(argv[0]));
    SFPath ExeDir = ExePath.parent_path();
    SFPath ConfigDir = (ExeDir / ".." / "Server").lexically_normal();
    SFString PluginConfigPath = "ServerPlugin.skywalkerC";
    SFString ServerConfigPath = "ServerConfig.skywalkerC";

    /** 允许通过命令行覆盖默认配置路径：argv[1]=Plugin，argv[2]=Server */
    if (argc > 1)
    {
        PluginConfigPath = argv[1];
    }
    if (argc > 2)
    {
        ServerConfigPath = argv[2];
    }

    PluginConfigPath = SkywalkerResolveConfigPath(PluginConfigPath, ExeDir, ConfigDir);
    ServerConfigPath = SkywalkerResolveConfigPath(ServerConfigPath, ExeDir, ConfigDir);

    /** 将配置路径注入环境变量，供框架启动阶段读取 */
    SkywalkerSetEnv("SKYWALKER_PLUGIN_CONFIG", PluginConfigPath.c_str());
    SkywalkerSetEnv("SKYWALKER_SERVER_CONFIG", ServerConfigPath.c_str());

    /** 进入框架主循环（生命周期由框架托管） */
    SKYWALKER_FRAMEWORK_START(argc, argv)

    return 0;
}
