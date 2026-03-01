/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Module\SSFModule_NetworkServer.cpp
**作者: shyfan
**日期: 2023/08/26 15:37:24
**功能: 网络服务器模块
*************************************************************************/

#include "SSFModule_NetworkServer.h"

#include "Include/SFCore.h"
#include "Include/SFILog.h"

#include "SkywalkerScript/Include/SkywalkerScriptParse.h"

SF_NAMESPACE_USING

SF_LOG_DEFINE(SSFModule_NetworkServer, Framework);

#pragma region Object

void SSFModule_NetworkServer::Init(SFObjectErrors &Errors)
{
    SSFModule::Init(Errors);
}

void SSFModule_NetworkServer::Awake(SFObjectErrors &Errors)
{
    SSFModule::Awake(Errors);
}

void SSFModule_NetworkServer::Start(SFObjectErrors &Errors)
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
                    continue;

                SKYWALKER_PTR_SCRIPT_NODE IPNode = ConfigNode->GetChildNodeFromName("IP");
                if (IPNode != nullptr)
                {
                    ServerIP = IPNode->GetNodeValueString();
                }

                SKYWALKER_PTR_SCRIPT_NODE PortNode = ConfigNode->GetChildNodeFromName("Port");
                if (PortNode != nullptr)
                {
                    ServerPort = std::stoi(PortNode->GetNodeValueString());
                }
            }
        }
    }

    StartNetworkServer(Errors);
}

void SSFModule_NetworkServer::Tick(SFObjectErrors &Errors, SFUInt64 DelayMS)
{
    SSFModule::Tick(Errors, DelayMS);

    CreateNetworkClient(Errors);

    SF_COMMON_ITERATOR(Iter, ClientNetworkSocketMap)
    {
        Iter->second->Tick(Errors, DelayMS);
    }
}

void SSFModule_NetworkServer::Stop(SFObjectErrors &Errors)
{
    StopNetworkServer(Errors);

    SSFModule::Stop(Errors);
}

void SSFModule_NetworkServer::Sleep(SFObjectErrors &Errors)
{
    SSFModule::Sleep(Errors);
}

void SSFModule_NetworkServer::Destroy(SFObjectErrors &Errors)
{
    SSFModule::Destroy(Errors);
}

#pragma endregion Object

void SSFModule_NetworkServer::StartNetworkServer(SFObjectErrors &InErrors)
{
    if (InErrors.IsValid())
    {
        return;
    }

    SSF_NETWORK_STARTUP();

    // 创建服务器套接字
    SSFNetworkSocketCreatorContext Context;
    Context.IP = ServerIP;
    Context.Port = ServerPort;
    auto pServerSocket = NewObject<SSFObject_ServerSocket>(Context, InErrors);
    ServerNetworkSocket = SF_UNIQUE_PTR_CAST(SSFObject_ServerSocket, pServerSocket);

    if (InErrors.IsValid())
    {
        SF_ERROR_DESC_TRACE(InErrors,
                            ESFError::Network_Start_Failed,
                            "Failed to start network server");
        SSF_NETWORK_CLEANUP();

        return;
    }

    SF_LOG_FRAMEWORK("Create ServerSocket ObjectGUID " << ServerNetworkSocket->GetObjectGUID() << " Socket " << ServerNetworkSocket->GetSocket());
}

void SSFModule_NetworkServer::StopNetworkServer(SFObjectErrors &Errors)
{
    ServerNetworkSocket->Stop(Errors);

    SSF_NETWORK_CLEANUP();
}

void SSFModule_NetworkServer::CreateNetworkClient(SFObjectErrors &Errors)
{
    if (ServerNetworkSocket->IsSocketInvalid())
    {
        SF_ERROR_DESC_TRACE(
            Errors,
            ESFError::Network_Socket_CreateFailed,
            "Create ClientSocket Failed, Because ServerSocket is Invalid")

        return;
    }

    SSFSOCKET ClientSocket = accept(ServerNetworkSocket->GetSocket(), NULL, NULL);
    if (ClientSocket == SSF_INVALID_SOCKET)
    {
        return;
    }

    // 创建客户端套接字
    SSFNetworkSocketCreatorContext Context;
    Context.Socket = ClientSocket;
    SSF_PRT_CLIENT_SOCKET ClientNetworkSocket = NewObject<SSFObject_ClientSocket>(Context, Errors);
    ClientNetworkSocketMap.insert(std::make_pair(ClientSocket, ClientNetworkSocket));

    SF_LOG_DEBUG("New ClientSocket: " << ClientSocket);
}
