/*************************************************************************
**文件: SkywalkerFramework\Plugin\CommandLine\Module\SFModule_OS.cpp
**作者: shyfan
**日期: 2024/01/16 14:46:57
**功能: 操作系统模块
*************************************************************************/

#include "SFModule_OS.h"

#include <csignal>
#include <signal.h>
#include <cstdio>

#if defined(SKYWALKER_PLATFORM_WINDOWS)
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFFramework.h"

#include "Core/Plugin/SFPlugin.h"
#include "Core/Plugin/SFPluginManager.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_OS, ESFLogLevel::Debug);

static SF_PTR(SFModule_OS) SelfModule{};

/* 缓冲输入行 */
static SFString StdinLineBuffer;

#pragma region Module

void SFModule_OS::Init(SFObjectErrors &Errors)
{
    SelfModule = this;

    /* 注册系统信号处理函数 */
    signal(SIGINT, &SFModule_OS::SignalHandler); /* Ctrl+C */
}

void SFModule_OS::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);

    NetworkServer = FindNetworkServer();
    SceneManager = FindSceneManager();

#if defined(SKYWALKER_PLATFORM_WINDOWS)
    /* 设置 stdin 为非阻塞模式 (Windows: 使用 _kbhit) */
#else
    /* 设置 stdin 为非阻塞模式 */
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
#endif
}

void SFModule_OS::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    SFString Line;
    if (TryReadStdinLine(Line))
    {
        ProcessCommand(Line);
    }
}

void SFModule_OS::Destroy(SFObjectErrors &Errors)
{
    SelfModule = nullptr;

    /* 注销系统信号处理函数 */
    signal(SIGINT, SIG_DFL); /* Ctrl+C */
}

#pragma endregion Module

SFModule_OS::SFModule_OS(SFModuleContext &InContext, SFObjectErrors &InErrors)
    : SSFModule(InContext, InErrors)
{
}

void SFModule_OS::SignalHandler(int Signal)
{
    SF_LOG_DEBUG("SignalHandler Signal: " << Signal);
    SelfModule->GetFramework()->Close();
}

bool SFModule_OS::TryReadStdinLine(SFString &OutLine)
{
#if defined(SKYWALKER_PLATFORM_WINDOWS)
    while (_kbhit())
    {
        int ch = _getch();
        if (ch == '\r' || ch == '\n')
        {
            if (!StdinLineBuffer.empty())
            {
                OutLine = StdinLineBuffer;
                StdinLineBuffer.clear();
                return true;
            }
        }
        else if (ch == '\b' || ch == 127)
        {
            if (!StdinLineBuffer.empty())
            {
                StdinLineBuffer.pop_back();
            }
        }
        else
        {
            StdinLineBuffer += static_cast<char>(ch);
        }
    }
    return false;
#else
    char Buf[256];
    int BytesRead = read(STDIN_FILENO, Buf, sizeof(Buf) - 1);
    if (BytesRead > 0)
    {
        Buf[BytesRead] = '\0';
        StdinLineBuffer += Buf;

        /* 查找换行 */
        auto Pos = StdinLineBuffer.find('\n');
        if (Pos != SFString::npos)
        {
            OutLine = StdinLineBuffer.substr(0, Pos);
            StdinLineBuffer = StdinLineBuffer.substr(Pos + 1);
            /* 去除可能的 \r */
            if (!OutLine.empty() && OutLine.back() == '\r')
            {
                OutLine.pop_back();
            }
            return !OutLine.empty();
        }
    }
    return false;
#endif
}

void SFModule_OS::ProcessCommand(const SFString &Command)
{
    if (Command == "status")
    {
        SF_LOG_FRAMEWORK("=== Server Status ===");

        if (NetworkServer != nullptr)
        {
            SF_LOG_FRAMEWORK("  Sessions: " << NetworkServer->GetSessionCount());
            SF_LOG_FRAMEWORK("  Online Players: " << NetworkServer->GetAuthenticatedSessionCount());
            SF_LOG_FRAMEWORK("  Recv Messages: " << NetworkServer->GetTotalRecvMsgCount());
            SF_LOG_FRAMEWORK("  Send Messages: " << NetworkServer->GetTotalSendMsgCount());
        }
        else
        {
            SF_LOG_FRAMEWORK("  NetworkServer: unavailable");
        }

        if (SceneManager != nullptr)
        {
            SFUInt32 DefaultSceneId = SceneManager->GetDefaultSceneId();
            std::vector<SFActorId> SceneActors = SceneManager->GetSceneActors(DefaultSceneId);
            SF_LOG_FRAMEWORK("  Scene " << DefaultSceneId << " Actors: " << SceneActors.size());
        }
        else
        {
            SF_LOG_FRAMEWORK("  SceneManager: unavailable");
        }

        SF_LOG_FRAMEWORK("=====================");
    }
    else if (Command == "quit" || Command == "exit" || Command == "stop")
    {
        SF_LOG_FRAMEWORK("Server shutting down by command...");
        GetFramework()->Close();
    }
    else if (Command == "help")
    {
        SF_LOG_FRAMEWORK("Available commands:");
        SF_LOG_FRAMEWORK("  status  - Show server status");
        SF_LOG_FRAMEWORK("  quit    - Shutdown server");
        SF_LOG_FRAMEWORK("  help    - Show this help");
    }
    else
    {
        SF_LOG_FRAMEWORK("Unknown command: " << Command << " (type 'help' for commands)");
    }
}

ISFNetworkServer *SFModule_OS::FindNetworkServer()
{
    SF_PTR(SFPlugin) OwnerPlugin = GetPlugin();
    if (SF_PTR_INVALID(OwnerPlugin))
    {
        return nullptr;
    }

    SF_PTR(SFPluginManager) PluginMgr = OwnerPlugin->GetPluginManager();
    if (SF_PTR_INVALID(PluginMgr))
    {
        return nullptr;
    }

    SFString PluginName = "Skywalker::Framework::SFPlugin_Network";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_NetworkServer";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFNetworkServer *>(Module);
}

ISFSceneManager *SFModule_OS::FindSceneManager()
{
    SF_PTR(SFPlugin) OwnerPlugin = GetPlugin();
    if (SF_PTR_INVALID(OwnerPlugin))
    {
        return nullptr;
    }

    SF_PTR(SFPluginManager) PluginMgr = OwnerPlugin->GetPluginManager();
    if (SF_PTR_INVALID(PluginMgr))
    {
        return nullptr;
    }

    SFString PluginName = "Skywalker::Framework::SFPlugin_Level";
    SF_PTR(SFPlugin) Plugin = PluginMgr->GetPlugin(PluginName);
    if (SF_PTR_INVALID(Plugin))
    {
        return nullptr;
    }

    SFString ModuleName = "Skywalker::Framework::SFModule_SceneManager";
    SF_PTR(SSFModule) Module = Plugin->GetModule(ModuleName);
    if (SF_PTR_INVALID(Module))
    {
        return nullptr;
    }

    return dynamic_cast<ISFSceneManager *>(Module);
}
