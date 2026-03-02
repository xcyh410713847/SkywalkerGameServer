/*************************************************************************
**文件: SkywalkerGameServer\SkywalkerNetworkTest.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 网络协议与持久化测试
*************************************************************************/

#include "SkywalkerTest/SkywalkerTest.h"

#include "SkywalkerFramework/Plugin/Network/Protocol/SSFNetworkCodec.h"
#include "SkywalkerFramework/Plugin/Network/Router/SSFNetworkRouter.h"
#include "SkywalkerFramework/Plugin/Network/Session/SSFNetworkBlacklistStore.h"

#include <filesystem>

SF_NAMESPACE_USING

SKYWALKER_TEST_SUITE(SkywalkerNetwork)

bool TestNetworkCodecRoundTrip()
{
    SSFNetworkPacket Packet;
    Packet.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::C2S_Login);
    Packet.Seq = 100;
    Packet.Body = {1, 2, 3, 4, 5};

    std::vector<SFByte> Encoded;
    SKYWALKER_TEST_ASSERT_TRUE(SSFNetworkCodec::Encode(Packet, Encoded));

    SSFNetworkCodec Codec;
    Codec.Append(Encoded.data(), 8);

    SSFNetworkPacket Decoded;
    SKYWALKER_TEST_ASSERT_FALSE(Codec.TryDecode(Decoded));
    SKYWALKER_TEST_ASSERT_EQ(Codec.GetLastDecodeError(), ESFNetworkDecodeError::NeedMoreData);

    Codec.Append(Encoded.data() + 8, static_cast<SFUInt32>(Encoded.size() - 8));
    SKYWALKER_TEST_ASSERT_TRUE(Codec.TryDecode(Decoded));
    SKYWALKER_TEST_ASSERT_EQ(Decoded.MsgId, Packet.MsgId);
    SKYWALKER_TEST_ASSERT_EQ(Decoded.Seq, Packet.Seq);
    SKYWALKER_TEST_ASSERT_EQ(Decoded.Body.size(), Packet.Body.size());

    return true;
}

bool TestNetworkCodecInvalidMagic()
{
    SSFNetworkPacket Packet;
    Packet.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::C2S_Heartbeat);

    std::vector<SFByte> Encoded;
    SKYWALKER_TEST_ASSERT_TRUE(SSFNetworkCodec::Encode(Packet, Encoded));
    SKYWALKER_TEST_ASSERT_TRUE(Encoded.size() >= 1);
    Encoded[0] = 0x00;

    SSFNetworkCodec Codec;
    Codec.Append(Encoded.data(), static_cast<SFUInt32>(Encoded.size()));

    SSFNetworkPacket Decoded;
    SKYWALKER_TEST_ASSERT_FALSE(Codec.TryDecode(Decoded));
    SKYWALKER_TEST_ASSERT_EQ(Codec.GetLastDecodeError(), ESFNetworkDecodeError::InvalidMagic);

    return true;
}

bool TestNetworkRouterDispatch()
{
    SSFNetworkRouter Router;
    bool bHandled = false;
    SFUInt32 HandledSeq = 0;

    Router.RegisterHandler(static_cast<SFUInt16>(ESFNetworkMsg::S2C_LoginAck),
                           [&bHandled, &HandledSeq](const SSFNetworkPacket &Packet)
                           {
                               bHandled = true;
                               HandledSeq = Packet.Seq;
                           });

    SSFNetworkPacket Packet;
    Packet.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::S2C_LoginAck);
    Packet.Seq = 777;

    SKYWALKER_TEST_ASSERT_TRUE(Router.Route(Packet));
    SKYWALKER_TEST_ASSERT_TRUE(bHandled);
    SKYWALKER_TEST_ASSERT_EQ(HandledSeq, 777u);

    return true;
}

bool TestNetworkRouterUnhandled()
{
    SSFNetworkRouter Router;

    SSFNetworkPacket Packet;
    Packet.MsgId = static_cast<SFUInt16>(ESFNetworkMsg::S2C_PlayerState);

    SKYWALKER_TEST_ASSERT_FALSE(Router.Route(Packet));
    return true;
}

bool TestBlacklistStoreSaveLoad()
{
    const std::filesystem::path TempPath = std::filesystem::temp_directory_path() / "skywalker_blacklist_test.data";

    SSFNetworkBlacklistSnapshot Snapshot;
    Snapshot.FormatVersion = 1;
    Snapshot.Entries["127.0.0.1"] = 2000;
    Snapshot.Entries["192.168.1.1"] = 500;

    std::filesystem::file_time_type SaveTime{};
    SKYWALKER_TEST_ASSERT_TRUE(SSFNetworkBlacklistStore::SaveToFile(TempPath.string(), Snapshot, &SaveTime));

    SSFNetworkBlacklistSnapshot Loaded;
    std::filesystem::file_time_type LoadTime{};
    SKYWALKER_TEST_ASSERT_TRUE(SSFNetworkBlacklistStore::LoadFromFile(TempPath.string(), 1000, Loaded, &LoadTime));

    SKYWALKER_TEST_ASSERT_EQ(Loaded.FormatVersion, 1u);
    SKYWALKER_TEST_ASSERT_EQ(Loaded.Entries.size(), static_cast<size_t>(1));
    SKYWALKER_TEST_ASSERT_TRUE(Loaded.Entries.find("127.0.0.1") != Loaded.Entries.end());

    std::error_code EC;
    std::filesystem::remove(TempPath, EC);

    return true;
}

SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkCodecRoundTrip, TestNetworkCodecRoundTrip)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkCodecInvalidMagic, TestNetworkCodecInvalidMagic)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkRouterDispatch, TestNetworkRouterDispatch)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkRouterUnhandled, TestNetworkRouterUnhandled)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestBlacklistStoreSaveLoad, TestBlacklistStoreSaveLoad)
