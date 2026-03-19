/*************************************************************************
**文件: SkywalkerFramework\Plugin\Network\Object\SFNetworkBuffer.h
**作者: shyfan
**日期: 2026/03/19
**功能: 网络收发线性缓冲区
*************************************************************************/

#ifndef __SKYWALKER_SERVER_FRAMEWORK_NETWORK_BUFFER_H__
#define __SKYWALKER_SERVER_FRAMEWORK_NETWORK_BUFFER_H__

#include "Include/SFCore.h"

#include <cstring>
#include <vector>

SF_NAMESPACE_BEGIN

/**
 * 线性网络缓冲区
 *
 * 用于 TCP 收发数据的累积与消费。
 * - Append: 追加数据到尾部
 * - Consume: 从头部消费指定字节
 * - Compact: 回收已消费空间（自动触发或手动调用）
 */
class SFNetworkBuffer
{
public:
    explicit SFNetworkBuffer(SFSize InitCapacity = 8192)
    {
        Data.resize(InitCapacity, 0);
        ReadPos = 0;
        WritePos = 0;
    }

    ~SFNetworkBuffer() = default;

    /** 可读字节数 */
    SFSize ReadableBytes() const
    {
        return WritePos - ReadPos;
    }

    /** 可写字节数（不扩容） */
    SFSize WritableBytes() const
    {
        return Data.size() - WritePos;
    }

    /** 可读数据起始指针 */
    const char *ReadPtr() const
    {
        return Data.data() + ReadPos;
    }

    /** 可写区域起始指针 */
    char *WritePtr()
    {
        return Data.data() + WritePos;
    }

    /** 追加数据到缓冲区尾部 */
    void Append(const char *InData, SFSize Len)
    {
        EnsureWritable(Len);
        std::memcpy(WritePtr(), InData, Len);
        WritePos += Len;
    }

    /** 推进写指针（配合直接写入 WritePtr 使用） */
    void AdvanceWrite(SFSize Len)
    {
        WritePos += Len;
    }

    /** 消费头部指定字节 */
    void Consume(SFSize Len)
    {
        if (Len >= ReadableBytes())
        {
            ReadPos = 0;
            WritePos = 0;
        }
        else
        {
            ReadPos += Len;
            /* 当已消费空间超过一半容量时自动回收 */
            if (ReadPos > Data.size() / 2)
            {
                Compact();
            }
        }
    }

    /** 回收已消费空间，将未读数据移至缓冲区头部 */
    void Compact()
    {
        SFSize Readable = ReadableBytes();
        if (ReadPos > 0 && Readable > 0)
        {
            std::memmove(Data.data(), Data.data() + ReadPos, Readable);
        }
        ReadPos = 0;
        WritePos = Readable;
    }

    /** 清空缓冲区 */
    void Clear()
    {
        ReadPos = 0;
        WritePos = 0;
    }

private:
    /** 确保可写空间足够，不足则扩容 */
    void EnsureWritable(SFSize Len)
    {
        if (WritableBytes() >= Len)
        {
            return;
        }

        /* 先尝试回收已消费空间 */
        Compact();
        if (WritableBytes() >= Len)
        {
            return;
        }

        /* 仍不够则扩容 */
        Data.resize(WritePos + Len);
    }

    std::vector<char> Data;
    SFSize ReadPos;
    SFSize WritePos;
};

SF_NAMESPACE_END

#endif /* __SKYWALKER_SERVER_FRAMEWORK_NETWORK_BUFFER_H__ */
