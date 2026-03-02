/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\Module\SSFModule_AIRuntime.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: AI运行时模块
*************************************************************************/

#include "SSFModule_AIRuntime.h"

#include "Include/SFILog.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_AIRuntime, ESFLogLevel::Debug);

void SSFModule_AIRuntime::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("AIRuntime module init, TickBudgetMS " << TickBudgetMS);
}

void SSFModule_AIRuntime::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, "SKYWALKER_SERVER_CONFIG");
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv("SKYWALKER_SERVER_CONFIG");
#endif
    SFString ServerConfigPath = ConfigPath ? ConfigPath : "ServerConfig.skywalkerC";
#if defined(_WIN32) || defined(_WIN64)
    if (ConfigPathBuffer != nullptr)
    {
        free(ConfigPathBuffer);
        ConfigPathBuffer = nullptr;
    }
#endif

    SKYWALKER_PTR_SCRIPT_PARSE ConfigParse = new SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse();
    if (ConfigParse->LoadScript(ServerConfigPath.c_str()))
    {
        SKYWALKER_PTR_SCRIPT_NODE RootNode = ConfigParse->GetRootNode();
        if (RootNode != nullptr)
        {
            for (size_t i = 0; i < RootNode->GetChildNodeNum(); i++)
            {
                SKYWALKER_PTR_SCRIPT_NODE ConfigNode = RootNode->GetChildNodeFromIndex(i);
                if (ConfigNode == nullptr)
                {
                    continue;
                }

                SKYWALKER_PTR_SCRIPT_NODE AITickBudgetNode = ConfigNode->GetChildNodeFromName("AITickBudgetMS");
                if (AITickBudgetNode != nullptr)
                {
                    SetTickBudgetMS(std::stoull(AITickBudgetNode->GetNodeValueString()));
                }
            }
        }
    }

    SF_LOG_FRAMEWORK("AIRuntime module start, TickBudgetMS " << TickBudgetMS);
}

void SSFModule_AIRuntime::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
    ++TickCounter;

    if (DelayMS > TickBudgetMS)
    {
        ++BudgetExceededCount;
        if (BudgetExceededCount % 100 == 0)
        {
            SF_LOG_FRAMEWORK("AIRuntime tick budget exceeded, DelayMS " << DelayMS
                                                                << " TickBudgetMS " << TickBudgetMS
                                                                << " ExceededCount " << BudgetExceededCount);
        }
    }
}

void SSFModule_AIRuntime::Stop(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("AIRuntime module stop, ExceededCount " << BudgetExceededCount);
    SSFModule::Stop(Errors);
}

void SSFModule_AIRuntime::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("AIRuntime module destroy, TickCounter " << TickCounter);
    SSFModule::Destroy(Errors);
}

void SSFModule_AIRuntime::SetTickBudgetMS(SFUInt64 InTickBudgetMS)
{
    TickBudgetMS = (InTickBudgetMS == 0) ? 1 : InTickBudgetMS;
}

SFUInt64 SSFModule_AIRuntime::GetTickBudgetMS() const
{
    return TickBudgetMS;
}

SFUInt64 SSFModule_AIRuntime::GetBudgetExceededCount() const
{
    return BudgetExceededCount;
}
