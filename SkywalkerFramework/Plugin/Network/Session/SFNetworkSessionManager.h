/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Session\SFNetworkSessionManager.h
**作者: shyfan
**日期: 2026/03/02
**功能: 网络会话管理
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_SESSION_MANAGER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_SESSION_MANAGER_H__

#include "SFNetworkSession.h"

SF_NAMESPACE_BEGIN

class SSFNetworkSessionManager
{
public:
    void AddSession(const SSFNetworkSession &Session);
    void RemoveSession(SSFSOCKET Socket);

    SSFNetworkSession *GetSession(SSFSOCKET Socket);
    SFMap<SSFSOCKET, SSFNetworkSession> &GetAllSessions();

private:
    SFMap<SSFSOCKET, SSFNetworkSession> SessionMap;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_NETWORK_SESSION_MANAGER_H__
