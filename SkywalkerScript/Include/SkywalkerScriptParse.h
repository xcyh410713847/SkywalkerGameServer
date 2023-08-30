/*************************************************************************
**文件: SkywalkerScript\Include\SkywalkerScriptParse.h
**作者: shyfan
**日期: 2023/08/29 14:56:55
**功能: Skywalker Script 解析器
*************************************************************************/

#ifndef __SKYWALKER_SCRIPT_PARSE_H__
#define __SKYWALKER_SCRIPT_PARSE_H__

#include <iostream>
#include <fstream>
#include <map>
#include <string>

#define SKYWALKER_SCRIPT_NAMESPACE Skywalker::Script
#define SKYWALKER_SCRIPT_NAMESPACE_BEGIN \
    namespace SKYWALKER_SCRIPT_NAMESPACE \
    {
#define SKYWALKER_SCRIPT_NAMESPACE_END } // namespace SKYWALKER_SCRIPT_NAMESPACE
#define SKYWALKER_SCRIPT_NAMESPACE_USE using namespace SKYWALKER_SCRIPT_NAMESPACE;

#define SKYWALKER_SCRIPT_PRINT(Content) std::cout << Content << std::endl;

SKYWALKER_SCRIPT_NAMESPACE_BEGIN

enum ESkywalkerLineParseResult
{
    SkywalkerLineParseResult_Failed = -1, // 失败
    SkywalkerLineParseResult_Success = 0, // 成功
    SkywalkerLineParseResult_Empty = 1,   // 空行
    SkywalkerLineParseResult_Comment = 2  // 注释
};

/**
 * 节点
 */
class CSkywalkerScriptNode
{
public:
    CSkywalkerScriptNode()
        : NodeName(nullptr), NodeValue(nullptr), ParentNode(nullptr)
    {
        ChildNodeMap.clear();
    }
    virtual ~CSkywalkerScriptNode()
    {
    }

    bool ParseNode(const char *NodeContent, int ContentLength)
    {
        if (nullptr == NodeContent)
        {
            return false;
        }

        char *NodeName = new char[ContentLength + 1];
        int NodeNameIndex = -1;
        char *NodeValue = new char[ContentLength + 1];
        int NodeValueIndex = -1;

        bool IsNodeName = true;

        for (int Index = 0; Index < ContentLength; ++Index)
        {
            char Char = NodeContent[Index];
            if (' ' == Char)
            {
                continue;
            }

            if ('=' == Char)
            {
                IsNodeName = false;
                continue;
            }

            if ('\r' == Char || '\n' == Char)
            {
                break;
            }

            if (IsNodeName)
            {
                NodeName[++NodeNameIndex] = Char;
            }
            else
            {
                NodeValue[++NodeValueIndex] = Char;
            }
        }

        NodeName[++NodeNameIndex] = '\0';
        NodeValue[++NodeValueIndex] = '\0';

        return InitNode(NodeName, NodeValue);
    }

    /**
     * 初始化节点
     */
    bool
    InitNode(const char *InNodeName, const char *InNodeValue)
    {
        NodeName = new char[strlen(InNodeName) + 1];
        strcpy_s(this->NodeName, strlen(InNodeName) + 1, InNodeName);

        this->NodeValue = new char[strlen(InNodeValue) + 1];
        strcpy_s(this->NodeValue, strlen(InNodeValue) + 1, InNodeValue);

        return true;
    }

    /**
     * 获取节点名称
     */
    const char *GetNodeName() const
    {
        return this->NodeName;
    }

#pragma region 父节点操作

public:
    /**
     * 设置父节点
     */
    void SetParentNode(CSkywalkerScriptNode *InParentNode)
    {
        this->ParentNode = InParentNode;
    }

    /**
     * 获取父节点
     */
    CSkywalkerScriptNode *GetParentNode() const
    {
        return this->ParentNode;
    }

#pragma endregion 父节点操作

#pragma region 子节点操作

public:
    /**
     * 添加子节点
     */
    void AddChildNode(CSkywalkerScriptNode *InChildNode)
    {
        this->ChildNodeMap.insert(std::make_pair(InChildNode->GetNodeName(), InChildNode));
    }

    /**
     * 获取子节点
     */
    CSkywalkerScriptNode *GetChildNode(const char *InChildNodeName) const
    {
        auto Iter = this->ChildNodeMap.find(InChildNodeName);
        if (Iter != this->ChildNodeMap.end())
        {
            return Iter->second;
        }

        return nullptr;
    }

#pragma endregion 子节点操作

#pragma region 获取节点值

public:
    /**
     * 获取Int类型的节点值
     */
    int GetNodeValueInt() const
    {
        return atoi(this->NodeValue);
    }

    /**
     * 获取Float类型的节点值
     */
    float GetNodeValueFloat() const
    {
        return atof(this->NodeValue);
    }

    /**
     * 获取String类型的节点值
     */
    const char *GetNodeValueString() const
    {
        return this->NodeValue;
    }

    /**
     * 获取Bool类型的节点值
     */
    bool GetNodeValueBool() const
    {
        return (0 == strcmp(this->NodeValue, "true")) ? true : false;
    }

#pragma endregion 获取节点值

private:
    char *NodeName;
    char *NodeValue;

    CSkywalkerScriptNode *ParentNode;                           // 父节点
    std::map<std::string, CSkywalkerScriptNode *> ChildNodeMap; // 子节点
};

/**
 * 解析器
 */
class CSkywalkerScriptParse
{
public:
    CSkywalkerScriptParse() {}
    virtual ~CSkywalkerScriptParse() {}

    /**
     * 加载文件
     */
    bool LoadScript(const char *InScriptName)
    {
        std::ifstream InFile(InScriptName);
        if (!InFile.is_open())
        {
            SKYWALKER_SCRIPT_PRINT("Open Script failed.");
            return false;
        }

        // 逐行读取
        std::string Line;
        int LineNum = 0;
        CSkywalkerScriptNode *LastNode = nullptr;
        int LastLayerLevel = 0;
        while (getline(InFile, Line))
        {
            ++LineNum;

            int LayerLevel = 0;
            CSkywalkerScriptNode *Node = new CSkywalkerScriptNode();
            int Result = LineParse(Line.c_str(), Line.length(), LayerLevel, Node);
            if (Result <= SkywalkerLineParseResult_Failed)
            {
                SKYWALKER_SCRIPT_PRINT("Line " << LineNum << " parse failed.");
                return false;
            }

            if (Result != SkywalkerLineParseResult_Success)
            {
                continue;
            }

            if (!RecursiveNode(LastLayerLevel, LastNode, LayerLevel, Node))
            {
                SKYWALKER_SCRIPT_PRINT("Line " << LineNum << " recursive failed.");
                return false;
            }

            LastNode = Node;
            LastLayerLevel = LayerLevel;
        }

        return true;
    }

private:
    /**
     * 行解析
     */
    int LineParse(const char *InLine, const int LineSize, int &LayerLevel, CSkywalkerScriptNode *OutNode)
    {
        if (nullptr == InLine)
        {
            return SkywalkerLineParseResult_Failed;
        }

        ESkywalkerLineParseResult Result = SkywalkerLineParseResult_Success;

        LayerLevel = 0;
        int SpaceNum = 0;
        char *Content = new char[LineSize + 1];
        int ContentIndex = -1;

        // 逐个字符解析
        for (int Index = 0; Index < LineSize; ++Index)
        {
            char Char = InLine[Index];
            if (' ' == Char)
            {
                ++SpaceNum;
                continue;
            }

            if ('\t' == Char)
            {
                SpaceNum += 4;
                continue;
            }

            if ('#' == Char)
            {
                Result = SkywalkerLineParseResult_Comment;
                break;
            }

            if ('\r' == Char || '\n' == Char)
            {
                Result = SkywalkerLineParseResult_Empty;
                break;
            }

            Content[++ContentIndex] = Char;
        }

        if (ContentIndex < 0)
        {
            Result = SkywalkerLineParseResult_Empty;
        }

        if (Result != SkywalkerLineParseResult_Success)
        {
            return Result;
        }

        // 计算层级
        LayerLevel = SpaceNum / 4;

        Content[++ContentIndex] = '\0';
        if (!OutNode->ParseNode(Content, ContentIndex + 1))
        {
            Result = SkywalkerLineParseResult_Failed;
        }

        return Result;
    }

    /**
     * 递归节点
     */
    bool RecursiveNode(int &LastLayerLevel, CSkywalkerScriptNode *LastNode, int &LayerLevel, CSkywalkerScriptNode *Node)
    {
        int LayerLevelDiff = LastLayerLevel - LayerLevel;
        if (LayerLevelDiff < -1)
        {
            SKYWALKER_SCRIPT_PRINT("跨级了，不允许跨级");
            return false;
        }

        // 子节点
        if (LayerLevelDiff == -1)
        {
            if (LastNode == nullptr)
            {
                SKYWALKER_SCRIPT_PRINT("父节点为空, 无法添加");
                return false;
            }
            Node->SetParentNode(LastNode);
            LastNode->AddChildNode(Node);

            return true;
        }

        // 上级节点
        while (LayerLevelDiff > 0)
        {
            LastNode = LastNode->GetParentNode();
            if (LastNode == nullptr)
            {
                SKYWALKER_SCRIPT_PRINT("父节点为空, 无法添加");
                return false;
            }

            --LayerLevelDiff;
        }

        // 同级节点
        if (LayerLevelDiff == 0)
        {
            if (LastLayerLevel == 0)
            {
                ScriptNodeMap.insert(std::make_pair(Node->GetNodeName(), Node));

                return true;
            }

            if (LastNode == nullptr)
            {
                SKYWALKER_SCRIPT_PRINT("同级节点为空, 无法添加");
                return false;
            }

            LastNode = LastNode->GetParentNode();
            if (LastNode == nullptr)
            {
                SKYWALKER_SCRIPT_PRINT("父节点为空, 无法添加");
                return false;
            }

            Node->SetParentNode(LastNode);
            LastNode->AddChildNode(Node);

            return true;
        }

        return true;
    }

private:
    std::map<std::string, CSkywalkerScriptNode *> ScriptNodeMap;
};

SKYWALKER_SCRIPT_NAMESPACE_END

#define SKYWALKER_PTR_SCRIPT_PARSE SKYWALKER_SCRIPT_NAMESPACE::CSkywalkerScriptParse *

#endif // __SKYWALKER_SCRIPT_PARSE_H__
