/*************************************************************************
**文件: SkywalkerFramework\Core\Object\SFJsonArchive.h
**作者: shyfan
**日期: 2026/03/17
**功能: JSON存档流实现
*************************************************************************/

#ifndef __SKYWALKER_FRAMEWORK_JSON_ARCHIVE_H__
#define __SKYWALKER_FRAMEWORK_JSON_ARCHIVE_H__

#include "SFArchive.h"
#include <map>
#include <vector>
#include <sstream>

SF_NAMESPACE_BEGIN

/**
 * JSON节点类型
 */
enum class ESFJsonNodeType
{
    None,
    Object,
    Array,
    String,
    Number,
    Bool,
    Null
};

/**
 * JSON节点
 */
class SFJsonNode
{
public:
    ESFJsonNodeType Type = ESFJsonNodeType::None;
    
    // 值存储
    SFString StringValue;
    SFInt64 IntValue = 0;
    SFDouble DoubleValue = 0.0;
    bool BoolValue = false;
    
    // 容器
    std::map<SFString, SFJsonNode> ObjectValue;
    std::vector<SFJsonNode> ArrayValue;

    SFJsonNode() = default;
    
    SFJsonNode(const SFString& InStr) : Type(ESFJsonNodeType::String), StringValue(InStr) {}
    SFJsonNode(SFInt64 InInt) : Type(ESFJsonNodeType::Number), IntValue(InInt) {}
    SFJsonNode(SFDouble InDouble) : Type(ESFJsonNodeType::Number), DoubleValue(InDouble) {}
    SFJsonNode(bool InBool) : Type(ESFJsonNodeType::Bool), BoolValue(InBool) {}

    /**
     * 获取字符串值
     */
    SFString ToString() const
    {
        switch (Type)
        {
        case ESFJsonNodeType::String:
            return StringValue;
        case ESFJsonNodeType::Number:
            return std::to_string(IntValue);
        case ESFJsonNodeType::Bool:
            return BoolValue ? "true" : "false";
        case ESFJsonNodeType::Null:
            return "null";
        default:
            return "";
        }
    }

    /**
     * 序列化为JSON字符串
     */
    SFString ToJsonString(SFInt32 Indent = 0) const
    {
        SFString IndentStr(Indent, ' ');
        
        switch (Type)
        {
        case ESFJsonNodeType::Object:
            {
                if (ObjectValue.empty())
                    return "{}";
                
                SFString Result = "{\n";
                SFInt32 Count = 0;
                for (const auto& Pair : ObjectValue)
                {
                    Count++;
                    Result += IndentStr + "  \"" + EscapeString(Pair.first) + "\": " + Pair.second.ToJsonString(Indent + 2);
                    if (Count < static_cast<SFInt32>(ObjectValue.size()))
                        Result += ",";
                    Result += "\n";
                }
                Result += IndentStr + "}";
                return Result;
            }
        case ESFJsonNodeType::Array:
            {
                if (ArrayValue.empty())
                    return "[]";
                
                SFString Result = "[";
                for (SFInt32 i = 0; i < static_cast<SFInt32>(ArrayValue.size()); i++)
                {
                    Result += ArrayValue[i].ToJsonString(Indent);
                    if (i < static_cast<SFInt32>(ArrayValue.size()) - 1)
                        Result += ", ";
                }
                Result += "]";
                return Result;
            }
        case ESFJsonNodeType::String:
            return "\"" + EscapeString(StringValue) + "\"";
        case ESFJsonNodeType::Number:
            return std::to_string(DoubleValue);
        case ESFJsonNodeType::Bool:
            return BoolValue ? "true" : "false";
        case ESFJsonNodeType::Null:
            return "null";
        default:
            return "null";
        }
    }

private:
    /**
     * 转义JSON字符串
     */
    SFString EscapeString(const SFString& InStr) const
    {
        SFString Result;
        for (char C : InStr)
        {
            switch (C)
            {
            case '\"': Result += "\\\""; break;
            case '\\': Result += "\\\\"; break;
            case '\n': Result += "\\n"; break;
            case '\r': Result += "\\r"; break;
            case '\t': Result += "\\t"; break;
            default: Result += C; break;
            }
        }
        return Result;
    }
};

/**
 * JSON存档流
 * 用于JSON格式的序列化/反序列化
 */
class SFJsonArchive : public SFArchive
{
public:
    SFJsonArchive()
        : bIsLoading(false)
        , CurrentNode(&RootNode)
    {
    }

    virtual ~SFJsonArchive() = default;

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

    /**
     * 获取JSON字符串
     */
    SFString ToJson() const
    {
        return RootNode.ToJsonString();
    }

    /**
     * 从JSON字符串加载
     */
    void LoadFromJson(const SFString& JsonStr)
    {
        RootNode = ParseJson(JsonStr);
        bIsLoading = true;
        CurrentPath.clear();
        CurrentNode = &RootNode;
    }

    /**
     * 获取根节点
     */
    SFJsonNode& GetRootNode() { return RootNode; }
    const SFJsonNode& GetRootNode() const { return RootNode; }

protected:
    /**
     * 序列化原始数据（JSON模式不支持，直接写入标识）
     */
    void SerializeRaw(void* Data, SFSize Size) override
    {
        // JSON模式下，将二进制数据转为Base64
        if (IsSaving())
        {
            SFString Base64 = "{\"_binary_\":\"";
            const SFByte* Bytes = static_cast<SFByte*>(Data);
            const char Base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            
            for (SFSize i = 0; i < Size; i += 3)
            {
                SFUInt32 Triple = Bytes[i] << 16;
                if (i + 1 < Size) Triple |= Bytes[i + 1] << 8;
                if (i + 2 < Size) Triple |= Bytes[i + 2];
                
                Base64 += Base64Chars[(Triple >> 18) & 0x3F];
                Base64 += Base64Chars[(Triple >> 12) & 0x3F];
                if (i + 1 < Size) Base64 += Base64Chars[(Triple >> 6) & 0x3F];
                if (i + 2 < Size) Base64 += Base64Chars[Triple & 0x3F];
            }
            Base64 += "\"}";
            
            // 保存到当前节点
            CurrentNode->Type = ESFJsonNodeType::String;
            CurrentNode->StringValue = Base64;
        }
        else
        {
            // 加载模式：从字符串还原
            // TODO: 实现Base64解码
        }
    }

    /**
     * 序列化字符串
     */
    void SerializeString(SFString& Str) override
    {
        if (IsSaving())
        {
            CurrentNode->Type = ESFJsonNodeType::String;
            CurrentNode->StringValue = Str;
        }
        else
        {
            if (CurrentNode->Type == ESFJsonNodeType::String)
            {
                Str = CurrentNode->StringValue;
            }
            else
            {
                Str.clear();
            }
        }
    }

    /**
     * 进入对象上下文
     */
    void EnterObject(const SFString& Key)
    {
        if (IsSaving())
        {
            CurrentNode->Type = ESFJsonNodeType::Object;
            auto It = CurrentNode->ObjectValue.find(Key);
            if (It == CurrentNode->ObjectValue.end())
            {
                CurrentNode->ObjectValue[Key] = SFJsonNode();
                CurrentNode = &CurrentNode->ObjectValue[Key];
            }
            else
            {
                CurrentNode = &It->second;
            }
        }
        else
        {
            auto It = CurrentNode->ObjectValue.find(Key);
            if (It != CurrentNode->ObjectValue.end())
            {
                CurrentNode = &It->second;
            }
        }
    }

    /**
     * 离开对象上下文
     */
    void LeaveObject()
    {
        // JSON存档不需要显式离开
    }

private:
    /** 根节点 */
    SFJsonNode RootNode;
    
    /** 当前节点 */
    SFJsonNode* CurrentNode = nullptr;
    
    /** 当前路径 */
    std::vector<SFString> CurrentPath;
    
    /** 是否为加载模式 */
    bool bIsLoading;

    /**
     * 简单JSON解析
     * 注意：这是一个极简实现，实际项目应使用专业JSON库
     */
    SFJsonNode ParseJson(const SFString& JsonStr)
    {
        SFJsonNode Root;
        const char* Str = JsonStr.c_str();
        SFSize Len = JsonStr.length();
        SFSize Pos = 0;
        
        // 跳过空白
        while (Pos < Len && isspace(Str[Pos])) Pos++;
        
        if (Pos < Len)
        {
            Root = ParseValue(Str, Len, Pos);
        }
        
        return Root;
    }

    SFJsonNode ParseValue(const char* Str, SFSize Len, SFSize& Pos)
    {
        // 跳过空白
        while (Pos < Len && isspace(Str[Pos])) Pos++;
        
        if (Pos >= Len) return SFJsonNode();

        switch (Str[Pos])
        {
        case '{': return ParseObject(Str, Len, Pos);
        case '[': return ParseArray(Str, Len, Pos);
        case '\"': return ParseString(Str, Len, Pos);
        case 't':
        case 'f': return ParseBool(Str, Len, Pos);
        case 'n': return ParseNull(Str, Len, Pos);
        default:
            if (Str[Pos] == '-' || isdigit(Str[Pos]))
                return ParseNumber(Str, Len, Pos);
            return SFJsonNode();
        }
    }

    SFJsonNode ParseObject(const char* Str, SFSize Len, SFSize& Pos)
    {
        SFJsonNode Node;
        Node.Type = ESFJsonNodeType::Object;
        
        Pos++; // skip '{'
        while (Pos < Len && Str[Pos] != '}')
        {
            // 跳过空白
            while (Pos < Len && isspace(Str[Pos])) Pos++;
            if (Pos >= Len || Str[Pos] == '}') break;
            
            // 解析key
            if (Str[Pos] == '\"')
            {
                SFJsonNode KeyNode = ParseString(Str, Len, Pos);
                SFString Key = KeyNode.StringValue;
                
                // 跳过空白和':'
                while (Pos < Len && isspace(Str[Pos])) Pos++;
                if (Pos < Len && Str[Pos] == ':') Pos++;
                
                // 解析value
                SFJsonNode ValueNode = ParseValue(Str, Len, Pos);
                Node.ObjectValue[Key] = ValueNode;
            }
            
            // 跳过空白和','
            while (Pos < Len && (isspace(Str[Pos]) || Str[Pos] == ',')) Pos++;
        }
        
        if (Pos < Len && Str[Pos] == '}') Pos++;
        return Node;
    }

    SFJsonNode ParseArray(const char* Str, SFSize Len, SFSize& Pos)
    {
        SFJsonNode Node;
        Node.Type = ESFJsonNodeType::Array;
        
        Pos++; // skip '['
        while (Pos < Len && Str[Pos] != ']')
        {
            SFJsonNode Item = ParseValue(Str, Len, Pos);
            Node.ArrayValue.push_back(Item);
            
            while (Pos < Len && (isspace(Str[Pos]) || Str[Pos] == ',')) Pos++;
        }
        
        if (Pos < Len && Str[Pos] == ']') Pos++;
        return Node;
    }

    SFJsonNode ParseString(const char* Str, SFSize Len, SFSize& Pos)
    {
        SFJsonNode Node;
        Node.Type = ESFJsonNodeType::String;
        
        Pos++; // skip '"'
        while (Pos < Len && Str[Pos] != '\"')
        {
            if (Str[Pos] == '\\' && Pos + 1 < Len)
            {
                Pos++;
                switch (Str[Pos])
                {
                case 'n': Node.StringValue += '\n'; break;
                case 'r': Node.StringValue += '\r'; break;
                case 't': Node.StringValue += '\t'; break;
                case '\"': Node.StringValue += '\"'; break;
                case '\\': Node.StringValue += '\\'; break;
                default: Node.StringValue += Str[Pos]; break;
                }
            }
            else
            {
                Node.StringValue += Str[Pos];
            }
            Pos++;
        }
        
        if (Pos < Len && Str[Pos] == '\"') Pos++;
        return Node;
    }

    SFJsonNode ParseNumber(const char* Str, SFSize Len, SFSize& Pos)
    {
        SFJsonNode Node;
        Node.Type = ESFJsonNodeType::Number;
        
        SFString NumStr;
        bool bHasDot = false;
        
        while (Pos < Len && (isdigit(Str[Pos]) || Str[Pos] == '.' || Str[Pos] == '-'))
        {
            if (Str[Pos] == '.') bHasDot = true;
            NumStr += Str[Pos];
            Pos++;
        }
        
        if (bHasDot)
        {
            Node.DoubleValue = std::stod(NumStr);
        }
        else
        {
            Node.IntValue = std::stoll(NumStr);
        }
        
        return Node;
    }

    SFJsonNode ParseBool(const char* Str, SFSize Len, SFSize& Pos)
    {
        SFJsonNode Node;
        Node.Type = ESFJsonNodeType::Bool;
        
        if (Len - Pos >= 4 && strncmp(Str + Pos, "true", 4) == 0)
        {
            Node.BoolValue = true;
            Pos += 4;
        }
        else if (Len - Pos >= 5 && strncmp(Str + Pos, "false", 5) == 0)
        {
            Node.BoolValue = false;
            Pos += 5;
        }
        
        return Node;
    }

    SFJsonNode ParseNull(const char* Str, SFSize Len, SFSize& Pos)
    {
        SFJsonNode Node;
        Node.Type = ESFJsonNodeType::Null;
        
        if (Len - Pos >= 4 && strncmp(Str + Pos, "null", 4) == 0)
        {
            Pos += 4;
        }
        
        return Node;
    }
};

SF_NAMESPACE_END

#endif // __SKYWALKER_FRAMEWORK_JSON_ARCHIVE_H__
