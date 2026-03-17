/*************************************************************************
**文件: SkywalkerFramework\Core\Object\SFMemoryArchive.h
**作者: shyfan
**日期: 2026/03/17
**功能: 内存存档流实现
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_MEMORY_ARCHIVE_H__
#define __SKYWALKER_FRAMEWORK_MEMORY_ARCHIVE_H__

#include "SFArchive.h"
#include <vector>
#include <cstring>

SF_NAMESPACE_BEGIN

/**
 * 内存存档流
 * 用于内存中的序列化/反序列化操作
 */
class SFMemoryArchive : public SFArchive
{
public:
    SFMemoryArchive()
        : Cursor(0)
        , bIsLoading(false)
    {
    }

    explicit SFMemoryArchive(const std::vector<SFByte>& InBuffer)
        : Buffer(InBuffer)
        , Cursor(0)
        , bIsLoading(true)
    {
    }

    virtual ~SFMemoryArchive() = default;

    /**
     * 判断是否为加载模式
     */
    bool IsLoading() const override { return bIsLoading; }

    /**
     * 判断是否为保存模式
     */
    bool IsSaving() const override { return !bIsLoading; }

    /**
     * 设置为加载模式
     */
    void SetLoading() { bIsLoading = true; }

    /**
     * 设置为保存模式
     */
    void SetSaving() { bIsLoading = false; }

protected:
    /**
     * 序列化原始数据
     */
    void SerializeRaw(void* Data, SFSize Size) override
    {
        if (IsLoading())
        {
            // 读取数据
            if (Cursor + Size <= Buffer.size())
            {
                std::memcpy(Data, Buffer.data() + Cursor, Size);
                Cursor += Size;
            }
            else
            {
                // 读取超出范围，填充零
                std::memset(Data, 0, Size);
                Cursor = Buffer.size();
            }
        }
        else
        {
            // 写入数据
            if (Cursor + Size > Buffer.size())
            {
                Buffer.resize(Cursor + Size);
            }
            std::memcpy(Buffer.data() + Cursor, Data, Size);
            Cursor += Size;
        }
    }

    /**
     * 序列化字符串
     */
    void SerializeString(SFString& Str) override
    {
        if (IsLoading())
        {
            // 读取长度
            SFSize Length = 0;
            SerializeRaw(&Length, sizeof(SFSize));

            // 读取字符串
            if (Length > 0 && Cursor + Length <= Buffer.size())
            {
                Str.assign(reinterpret_cast<const char*>(Buffer.data() + Cursor), Length);
                Cursor += Length;
            }
            else
            {
                Str.clear();
            }
        }
        else
        {
            // 写入长度
            SFSize Length = Str.length();
            SerializeRaw(&Length, sizeof(SFSize));

            // 写入字符串
            if (Length > 0)
            {
                SFSize OldSize = Buffer.size();
                Buffer.resize(OldSize + Length);
                std::memcpy(Buffer.data() + OldSize, Str.c_str(), Length);
                Cursor += Length;
            }
        }
    }

    /**
     * 获取底层缓冲区
     */
    const std::vector<SFByte>& GetBuffer() const { return Buffer; }

    /**
     * 获取缓冲区大小
     */
    SFSize GetSize() const { return Buffer.size(); }

    /**
     * 获取当前位置
     */
    SFSize GetCursor() const { return Cursor; }

    /**
     * 转换为JSON字符串
     */
    SFString ToJson() const
    {
        // 简单实现：将二进制转为Base64或十六进制字符串
        // 实际项目中可以使用更复杂的JSON序列化
        SFString Result;
        Result.reserve(Buffer.size() * 2);
        
        const char HexChars[] = "0123456789ABCDEF";
        for (SFByte Byte : Buffer)
        {
            Result += HexChars[(Byte >> 4) & 0x0F];
            Result += HexChars[Byte & 0x0F];
        }
        return Result;
    }

    /**
     * 从JSON字符串加载
     */
    void LoadFromJson(const SFString& JsonStr)
    {
        // 简单实现：从十六进制字符串解析
        Buffer.clear();
        Cursor = 0;
        
        SFSize HexLen = JsonStr.length();
        if (HexLen % 2 != 0)
        {
            return;
        }
        
        Buffer.reserve(HexLen / 2);
        for (SFSize i = 0; i < HexLen; i += 2)
        {
            char High = JsonStr[i];
            char Low = JsonStr[i + 1];
            
            SFByte Byte = 0;
            if (High >= '0' && High <= '9')
                Byte = (High - '0') << 4;
            else if (High >= 'A' && High <= 'F')
                Byte = (High - 'A' + 10) << 4;
            else if (High >= 'a' && High <= 'f')
                Byte = (High - 'a' + 10) << 4;
                
            if (Low >= '0' && Low <= '9')
                Byte |= (Low - '0');
            else if (Low >= 'A' && Low <= 'F')
                Byte |= (Low - 'A' + 10);
            else if (Low >= 'a' && Low <= 'f')
                Byte |= (Low - 'a' + 10);
                
            Buffer.push_back(Byte);
        }
        
        bIsLoading = true;
        Cursor = 0;
    }

    /**
     * 清空缓冲区
     */
    void Clear()
    {
        Buffer.clear();
        Cursor = 0;
    }

private:
    /** 内存缓冲区 */
    std::vector<SFByte> Buffer;
    
    /** 读写位置指针 */
    SFSize Cursor;
    
    /** 是否为加载模式 */
    bool bIsLoading;
};

SF_NAMESPACE_END

#endif // __SKYWALKER_FRAMEWORK_MEMORY_ARCHIVE_H__
