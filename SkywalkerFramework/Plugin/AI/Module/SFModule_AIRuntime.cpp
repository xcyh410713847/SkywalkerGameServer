/*************************************************************************
**文件: SkywalkerFramework\Plugin\AI\Module\SFModule_AIRuntime.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: AI运行时模块
*************************************************************************/

#include "SFModule_AIRuntime.h"

#include "Include/SFILog.h"

#include "Core/Service/SFGameplayServiceGateway.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

#include <sstream>

SF_NAMESPACE_USING

SF_LOG_DEFINE(SFModule_AIRuntime, ESFLogLevel::Debug);

void SFModule_AIRuntime::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
    SF_LOG_FRAMEWORK("AIRuntime module init, TickBudgetMS " << TickBudgetMS);
}

void SFModule_AIRuntime::Start(SFObjectErrors &Errors)
{
    SSFModule::Start(Errors);

    const char *ConfigPath = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    char *ConfigPathBuffer = nullptr;
    size_t ConfigPathLen = 0;
    _dupenv_s(&ConfigPathBuffer, &ConfigPathLen, SF_ENV_PROGRAM_CONFIG_DIR);
    ConfigPath = ConfigPathBuffer;
#else
    ConfigPath = getenv(SF_ENV_PROGRAM_CONFIG_DIR);
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

                SKYWALKER_PTR_SCRIPT_NODE AIStrategyNode = ConfigNode->GetChildNodeFromName("AIStrategy");
                if (AIStrategyNode != nullptr)
                {
                    SetStrategy(AIStrategyNode->GetNodeValueString());
                }
            }
        }
    }

    SSFGameplayServiceGateway::Instance().RegisterAISetStrategy(
        [this](const SFString &InStrategyName)
        {
            return SetStrategy(InStrategyName);
        });

    SSFGameplayServiceGateway::Instance().RegisterAIGetStats(
        [this]()
        {
            return BuildStats();
        });

    SSFGameplayServiceGateway::Instance().RegisterAIGetStrategies(
        [this]()
        {
            return BuildStrategies();
        });

    SSFGameplayServiceGateway::Instance().RegisterAIGetAudit(
        [this]()
        {
            return BuildAudit();
        });

    SSFGameplayServiceGateway::Instance().RegisterAIClearAudit(
        [this]()
        {
            return ClearAudit();
        });

    SF_LOG_FRAMEWORK("AIRuntime module start, TickBudgetMS " << TickBudgetMS << " Strategy " << StrategyName);
}

void SFModule_AIRuntime::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);
    ++TickCounter;

    SFUInt64 EffectiveBudget = GetEffectiveBudgetMS();
    if (DelayMS > EffectiveBudget)
    {
        ++BudgetExceededCount;
        if (BudgetExceededCount % 100 == 0)
        {
            SF_LOG_FRAMEWORK("AIRuntime tick budget exceeded, DelayMS " << DelayMS
                                                                        << " EffectiveBudgetMS " << EffectiveBudget
                                                                        << " Strategy " << StrategyName
                                                                        << " ExceededCount " << BudgetExceededCount);
        }
    }
}

void SFModule_AIRuntime::Stop(SFObjectErrors &Errors)
{
    SSFGameplayServiceGateway::Instance().RegisterAISetStrategy(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterAIGetStats(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterAIGetStrategies(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterAIGetAudit(nullptr);
    SSFGameplayServiceGateway::Instance().RegisterAIClearAudit(nullptr);

    SF_LOG_FRAMEWORK("AIRuntime module stop, ExceededCount " << BudgetExceededCount);
    SSFModule::Stop(Errors);
}

void SFModule_AIRuntime::Destroy(SFObjectErrors &Errors)
{
    SF_LOG_FRAMEWORK("AIRuntime module destroy, TickCounter " << TickCounter);
    SSFModule::Destroy(Errors);
}

void SFModule_AIRuntime::SetTickBudgetMS(SFUInt64 InTickBudgetMS)
{
    TickBudgetMS = (InTickBudgetMS == 0) ? 1 : InTickBudgetMS;
}

SFUInt64 SFModule_AIRuntime::GetTickBudgetMS() const
{
    return TickBudgetMS;
}

SFUInt64 SFModule_AIRuntime::GetBudgetExceededCount() const
{
    return BudgetExceededCount;
}

bool SFModule_AIRuntime::SetStrategy(const SFString &InStrategyName)
{
    if (SupportedStrategies.find(InStrategyName) == SupportedStrategies.end())
    {
        return false;
    }

    if (StrategyName == InStrategyName)
    {
        return true;
    }

    SFString OldStrategy = StrategyName;
    StrategyName = InStrategyName;
    AppendStrategyAudit(OldStrategy, StrategyName);
    return true;
}

const SFString &SFModule_AIRuntime::GetStrategy() const
{
    return StrategyName;
}

SFString SFModule_AIRuntime::BuildStats() const
{
    return "AIStrategy=" + StrategyName +
           ";TickBudgetMS=" + std::to_string(TickBudgetMS) +
           ";EffectiveBudgetMS=" + std::to_string(GetEffectiveBudgetMS()) +
           ";TickCounter=" + std::to_string(TickCounter) +
           ";BudgetExceededCount=" + std::to_string(BudgetExceededCount);
}

SFString SFModule_AIRuntime::BuildStrategies() const
{
    std::ostringstream Stream;
    Stream << "AIStrategies=";
    for (size_t i = 0; i < StrategyOrder.size(); ++i)
    {
        if (i > 0)
        {
            Stream << "|";
        }

        Stream << StrategyOrder[i];
    }

    return Stream.str();
}

SFString SFModule_AIRuntime::BuildAudit() const
{
    if (StrategyAudit.empty())
    {
        return "AIStrategyAudit=Empty";
    }

    std::ostringstream Stream;
    Stream << "AIStrategyAudit=";
    for (size_t i = 0; i < StrategyAudit.size(); ++i)
    {
        if (i > 0)
        {
            Stream << "|";
        }

        Stream << StrategyAudit[i];
    }

    return Stream.str();
}

bool SFModule_AIRuntime::ClearAudit()
{
    StrategyAudit.clear();
    return true;
}

SFUInt64 SFModule_AIRuntime::GetEffectiveBudgetMS() const
{
    if (StrategyName == "balanced")
    {
        return (TickBudgetMS * 3) / 2;
    }

    if (StrategyName == "relaxed")
    {
        return TickBudgetMS * 2;
    }

    return TickBudgetMS;
}

void SFModule_AIRuntime::AppendStrategyAudit(const SFString &FromStrategy, const SFString &ToStrategy)
{
    ++StrategySwitchSeq;
    SFString Record = "Seq=" + std::to_string(StrategySwitchSeq) +
                      ";From=" + FromStrategy +
                      ";To=" + ToStrategy +
                      ";TickCounter=" + std::to_string(TickCounter);

    StrategyAudit.push_back(Record);
    if (StrategyAudit.size() > 16)
    {
        StrategyAudit.erase(StrategyAudit.begin());
    }
}
