/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Session\SFNetworkSessionManager.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 网络会话管理
*************************************************************************/

#include "SFNetworkSessionManager.h"

SF_NAMESPACE_USING

void SSFNetworkSessionManager::AddSession(const SSFNetworkSession &Session)
{
    SessionMap[Session.Socket] = Session;
}

void SSFNetworkSessionManager::RemoveSession(SSFSOCKET Socket)
{
    SessionMap.erase(Socket);
}

SSFNetworkSession *SSFNetworkSessionManager::GetSession(SSFSOCKET Socket)
{
    auto Iter = SessionMap.find(Socket);
    if (Iter == SessionMap.end())
    {
        return nullptr;
    }

    return &Iter->second;
}

SFMap<SSFSOCKET, SSFNetworkSession> &SSFNetworkSessionManager::GetAllSessions()
{
    return SessionMap;
}
