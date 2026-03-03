/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Protocol\SFNetworkCodec.h
**作者: shyfan
**日期: 2026/03/02
**功能: 网络包编解码
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_CODEC_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_CODEC_H__

#include "SFNetworkPacket.h"

#include <vector>

SF_NAMESPACE_BEGIN

enum class ESFNetworkDecodeError
{
    None = 0,
    NeedMoreData,
    InvalidMagic,
    InvalidVersion,
    BodyTooLarge,
};

class SSFNetworkCodec
{
public:
    /**
     * 追加接收缓冲
     */
    void Append(SF_CONST_PTR(SFByte) Data, SFUInt32 Length);

    /**
     * 尝试解一个包
     */
    bool TryDecode(SSFNetworkPacket &Packet);

    /**
     * 编码包
     */
    static bool Encode(const SSFNetworkPacket &Packet, std::vector<SFByte> &OutBuffer);

    ESFNetworkDecodeError GetLastDecodeError() const
    {
        return LastDecodeError;
    }

    bool HasFatalError() const
    {
        return bFatalError;
    }

    void ResetFatalError()
    {
        bFatalError = false;
    }

private:
    std::vector<SFByte> RecvBuffer;
    ESFNetworkDecodeError LastDecodeError = ESFNetworkDecodeError::None;
    bool bFatalError = false;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_SERVER_FRAMEWORK_NETWORK_CODEC_H__
