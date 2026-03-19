/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFMessageDispatcher.h
**作者: shyfan
**日期: 2026/03/19
**功能: 消息分发器
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_MESSAGE_DISPATCHER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_MESSAGE_DISPATCHER_H__

#include "Include/SFCore.h"
#include "Include/SFILog.h"
#include "Include/SFNetworkInterface.h"

#include "SFMessageCodec.h"

#include <functional>

SF_NAMESPACE_BEGIN

/**
 * 消息分发器
 *
 * 其他插件的模块在 Init() 阶段通过 RegisterHandler 注册消息处理回调，
 * NetworkServer 在收到完整消息后调用 Dispatch 分发。
 *
 * SFMessageHandler 类型定义在 Include/SFNetworkInterface.h 中。
 */
class SFMessageDispatcher
{
public:
    SFMessageDispatcher() = default;
    ~SFMessageDispatcher() = default;

    /**
     * 注册消息处理器
     * @param MsgID   消息ID
     * @param Handler 回调函数
     * @return true=注册成功, false=已有同 MsgID 的处理器
     */
    bool RegisterHandler(SFMsgID MsgID, SFMessageHandler Handler)
    {
        if (HandlerMap.find(MsgID) != HandlerMap.end())
        {
            return false;
        }
        HandlerMap.insert(std::make_pair(MsgID, Handler));
        return true;
    }

    /**
     * 注销消息处理器
     * @param MsgID 消息ID
     */
    void UnregisterHandler(SFMsgID MsgID)
    {
        auto Iter = HandlerMap.find(MsgID);
        if (Iter != HandlerMap.end())
        {
            HandlerMap.erase(Iter);
        }
    }

    /**
     * 分发一条消息
     * @param SessionId  会话ID
     * @param MsgID      消息ID
     * @param Payload    负载数据
     * @param PayloadLen 负载长度
     * @return true=找到处理器并分发, false=无对应处理器
     */
    bool Dispatch(SFUInt32 SessionId, SFMsgID MsgID,
                  const char *Payload, SFUInt32 PayloadLen)
    {
        auto Iter = HandlerMap.find(MsgID);
        if (Iter == HandlerMap.end())
        {
            return false;
        }
        Iter->second(SessionId, Payload, PayloadLen);
        return true;
    }

    /**
     * 是否已注册指定消息ID的处理器
     */
    bool HasHandler(SFMsgID MsgID) const
    {
        return HandlerMap.find(MsgID) != HandlerMap.end();
    }

private:
    SFMap<SFMsgID, SFMessageHandler> HandlerMap;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_MESSAGE_DISPATCHER_H__ */
