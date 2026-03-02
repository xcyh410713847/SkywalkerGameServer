/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Session\SSFNetworkSession.h
**作者: shyfan
**日期: 2026/03/02
**功能: 网络会话
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_SESSION_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_SESSION_H__

#include "Include/SFCore.h"

#include "../Object/SSFObject_NetworkSocket.h"

SF_NAMESPACE_BEGIN

struct SSFNetworkSession
{
    SFUInt64 SessionId = 0;
    SSFSOCKET Socket = SSF_INVALID_SOCKET;
    SFString ClientIP;

    SFUInt64 LastRecvMS = 0;
    SFUInt64 LastSendMS = 0;
    SFUInt64 LastHeartbeatMS = 0;

    SFUInt64 RateWindowStartMS = 0;
    SFUInt32 RateWindowMsgCount = 0;

    SFBool IsAuthed = FALSE;

    SFUInt64 DroppedMsgCount = 0;
    SFUInt64 InvalidPacketCount = 0;
    SFUInt64 ViolationCount = 0;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_NETWORK_SESSION_H__
