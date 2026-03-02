/*************************************************************************
**文件: SkywalkerGameServer\SkywalkerNetworkTest.cpp
**作者: shyfan
**日期: 2026/03/02
**功能: 网络协议与持久化测试
*************************************************************************/

#include "SkywalkerTest/SkywalkerTest.h"

#include "SkywalkerFramework/Plugin/Network/Protocol/SSFNetworkCodec.h"
#include "SkywalkerFramework/Plugin/Network/Protocol/SSFNetworkLoginPayload.h"
#include "SkywalkerFramework/Plugin/Network/Router/SSFNetworkRouter.h"
#include "SkywalkerFramework/Plugin/Network/Session/SSFNetworkBlacklistStore.h"
#include "SkywalkerFramework/Plugin/Level/Module/SSFModule_WorldManager.h"
#include "SkywalkerFramework/Plugin/CommandLine/Module/SSFModule_AdminCommand.h"
#include "SkywalkerFramework/Plugin/AI/Module/SSFModule_AIRuntime.h"
#include "SkywalkerFramework/Plugin/Video/Module/SSFModule_ReplayRecorder.h"
#include "SkywalkerFramework/Plugin/Video/Module/SSFModule_ReplayPlayer.h"
#include "SkywalkerFramework/Core/Service/SSFGameplayServiceGateway.h"

#include <filesystem>
#include <fstream>

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

bool TestLoginPayloadCodecRoundTrip()
{
    SSFNetworkLoginPayload Payload;
    Payload.PlayerId = 10001;
    Payload.WorldId = 1;
    Payload.Token = "Token-10001";

    std::vector<SFByte> Body;
    SKYWALKER_TEST_ASSERT_TRUE(SSFNetworkLoginPayloadCodec::Encode(Payload, Body));

    SSFNetworkLoginPayload Decoded;
    SKYWALKER_TEST_ASSERT_TRUE(SSFNetworkLoginPayloadCodec::Decode(Body, Decoded));
    SKYWALKER_TEST_ASSERT_EQ(Decoded.PlayerId, Payload.PlayerId);
    SKYWALKER_TEST_ASSERT_EQ(Decoded.WorldId, Payload.WorldId);
    SKYWALKER_TEST_ASSERT_EQ(Decoded.Token, Payload.Token);

    return true;
}

bool TestLoginPayloadCodecInvalid()
{
    std::vector<SFByte> Body = {'1', '2', '3'};

    SSFNetworkLoginPayload Decoded;
    SKYWALKER_TEST_ASSERT_FALSE(SSFNetworkLoginPayloadCodec::Decode(Body, Decoded));
    return true;
}

bool TestWorldManagerEnterLeave()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    SSFModule_WorldManager WorldManager(Context, Errors);

    SKYWALKER_TEST_ASSERT_TRUE(WorldManager.CreateWorld(1));
    SKYWALKER_TEST_ASSERT_TRUE(WorldManager.EnterWorld(10001, 1));
    SKYWALKER_TEST_ASSERT_TRUE(WorldManager.EnterWorld(10002, 1));
    SKYWALKER_TEST_ASSERT_EQ(WorldManager.GetWorldPlayerCount(1), static_cast<SFUInt64>(2));

    SKYWALKER_TEST_ASSERT_TRUE(WorldManager.LeaveWorld(10001, 1));
    SKYWALKER_TEST_ASSERT_EQ(WorldManager.GetWorldPlayerCount(1), static_cast<SFUInt64>(1));

    return true;
}

bool TestWorldManagerSwitchWorld()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    SSFModule_WorldManager WorldManager(Context, Errors);

    SKYWALKER_TEST_ASSERT_TRUE(WorldManager.EnterWorld(10001, 1));
    SKYWALKER_TEST_ASSERT_TRUE(WorldManager.EnterWorld(10001, 2));
    SKYWALKER_TEST_ASSERT_EQ(WorldManager.GetWorldPlayerCount(1), static_cast<SFUInt64>(0));
    SKYWALKER_TEST_ASSERT_EQ(WorldManager.GetWorldPlayerCount(2), static_cast<SFUInt64>(1));

    return true;
}

bool TestWorldManagerInvalidInput()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    SSFModule_WorldManager WorldManager(Context, Errors);

    SKYWALKER_TEST_ASSERT_FALSE(WorldManager.CreateWorld(0));
    SKYWALKER_TEST_ASSERT_FALSE(WorldManager.EnterWorld(0, 1));
    SKYWALKER_TEST_ASSERT_FALSE(WorldManager.EnterWorld(10001, 0));
    SKYWALKER_TEST_ASSERT_FALSE(WorldManager.LeaveWorld(10001, 1));

    return true;
}

bool TestAdminCommandExecute()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    SSFModule_AdminCommand AdminCommand(Context, Errors);
    AdminCommand.Init(Errors);

    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("show_stats"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("kick_player 10001"));
    SKYWALKER_TEST_ASSERT_FALSE(AdminCommand.ExecuteCommand("unknown_cmd"));
    SKYWALKER_TEST_ASSERT_FALSE(AdminCommand.ExecuteCommand(""));

    AdminCommand.Destroy(Errors);
    return true;
}

bool TestGameplayServiceGatewayCallbacks()
{
    auto &Gateway = SSFGameplayServiceGateway::Instance();

    Gateway.RegisterAuthValidator([](const SFString &Token)
                                  { return Token == "Token-10001"; });
    Gateway.RegisterPlayerLoader([](SFUInt64 PlayerId)
                                 { return PlayerId == 10001; });
    Gateway.RegisterWorldEnter([](SFUInt64 PlayerId, SFUInt64 WorldId)
                               { return PlayerId == 10001 && WorldId == 1; });
    Gateway.RegisterWorldLeave([](SFUInt64 PlayerId, SFUInt64 WorldId)
                               { return PlayerId == 10001 && WorldId == 1; });
    Gateway.RegisterReplayStartRecord([](SFUInt64 SessionId)
                                      { return SessionId == 10001; });
    Gateway.RegisterReplayStopRecord([]()
                                     { return true; });
    Gateway.RegisterReplayStartPlay([](SFUInt64 SessionId)
                                    { return SessionId == 10001; });
    Gateway.RegisterReplayStopPlay([]()
                                   { return true; });
    Gateway.RegisterReplayRecordStats([]()
                                      { return SFString("ReplayRecordStats=Ready"); });
    Gateway.RegisterReplayPlayStats([]()
                                    { return SFString("ReplayPlayStats=Ready"); });
    Gateway.RegisterAISetStrategy([](const SFString &StrategyName)
                                  { return StrategyName == "strict" || StrategyName == "relaxed"; });
    Gateway.RegisterAIGetStats([]()
                               { return SFString("AIStrategy=strict"); });
    Gateway.RegisterAIGetStrategies([]()
                                    { return SFString("AIStrategies=strict|balanced|relaxed"); });

    SKYWALKER_TEST_ASSERT_TRUE(Gateway.ValidateToken("Token-10001"));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.ValidateToken("Token-Other"));
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.LoadPlayer(10001));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.LoadPlayer(20002));
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.EnterWorld(10001, 1));
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.LeaveWorld(10001, 1));
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.StartReplayRecord(10001));
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.StopReplayRecord());
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.StartReplayPlay(10001));
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.StopReplayPlay());
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetReplayRecordStats(), SFString("ReplayRecordStats=Ready"));
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetReplayPlayStats(), SFString("ReplayPlayStats=Ready"));
    SKYWALKER_TEST_ASSERT_TRUE(Gateway.SetAIStrategy("strict"));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.SetAIStrategy("unsupported"));
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetAIStats(), SFString("AIStrategy=strict"));
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetAIStrategies(), SFString("AIStrategies=strict|balanced|relaxed"));

    Gateway.RegisterAuthValidator(nullptr);
    Gateway.RegisterPlayerLoader(nullptr);
    Gateway.RegisterWorldEnter(nullptr);
    Gateway.RegisterWorldLeave(nullptr);
    Gateway.RegisterReplayStartRecord(nullptr);
    Gateway.RegisterReplayStopRecord(nullptr);
    Gateway.RegisterReplayStartPlay(nullptr);
    Gateway.RegisterReplayStopPlay(nullptr);
    Gateway.RegisterReplayRecordStats(nullptr);
    Gateway.RegisterReplayPlayStats(nullptr);
    Gateway.RegisterAISetStrategy(nullptr);
    Gateway.RegisterAIGetStats(nullptr);
    Gateway.RegisterAIGetStrategies(nullptr);

    SKYWALKER_TEST_ASSERT_FALSE(Gateway.ValidateToken("Token-10001"));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.LoadPlayer(10001));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.EnterWorld(10001, 1));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.LeaveWorld(10001, 1));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.StartReplayRecord(10001));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.StopReplayRecord());
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.StartReplayPlay(10001));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.StopReplayPlay());
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetReplayRecordStats(), SFString("ReplayRecordStatsUnavailable"));
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetReplayPlayStats(), SFString("ReplayPlayStatsUnavailable"));
    SKYWALKER_TEST_ASSERT_FALSE(Gateway.SetAIStrategy("strict"));
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetAIStats(), SFString("AIStatsUnavailable"));
    SKYWALKER_TEST_ASSERT_EQ(Gateway.GetAIStrategies(), SFString("AIStrategiesUnavailable"));

    return true;
}

bool TestAIRuntimeTickBudget()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    SSFModule_AIRuntime AIRuntime(Context, Errors);

    AIRuntime.SetTickBudgetMS(5);
    SKYWALKER_TEST_ASSERT_EQ(AIRuntime.GetTickBudgetMS(), static_cast<SFUInt64>(5));
    SKYWALKER_TEST_ASSERT_EQ(AIRuntime.GetStrategy(), SFString("strict"));
    SKYWALKER_TEST_ASSERT_EQ(AIRuntime.BuildStrategies(), SFString("AIStrategies=strict|balanced|relaxed"));
    SKYWALKER_TEST_ASSERT_TRUE(AIRuntime.SetStrategy("relaxed"));
    SKYWALKER_TEST_ASSERT_TRUE(AIRuntime.SetStrategy("balanced"));
    SKYWALKER_TEST_ASSERT_FALSE(AIRuntime.SetStrategy("invalid"));

    AIRuntime.Tick(Errors, 10);
    AIRuntime.Tick(Errors, 1);
    SKYWALKER_TEST_ASSERT_EQ(AIRuntime.GetBudgetExceededCount(), static_cast<SFUInt64>(1));

    AIRuntime.SetStrategy("strict");
    AIRuntime.Tick(Errors, 10);
    SKYWALKER_TEST_ASSERT_EQ(AIRuntime.GetBudgetExceededCount(), static_cast<SFUInt64>(1));

    SFString AIStats = AIRuntime.BuildStats();
    SKYWALKER_TEST_ASSERT_TRUE(AIStats.find("AIStrategy=strict") != SFString::npos);

    return true;
}

bool TestReplayRecorderState()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    SSFModule_ReplayRecorder Recorder(Context, Errors);

    const std::filesystem::path ReplayDir = std::filesystem::temp_directory_path() / "skywalker_replay_test";
    Recorder.SetReplayDirectory(ReplayDir.string());

    SKYWALKER_TEST_ASSERT_FALSE(Recorder.IsRecording());
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.StartRecord(101));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.RecordEvent("Frame=1"));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.RecordEvent("Frame=2"));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.IsRecording());
    SKYWALKER_TEST_ASSERT_FALSE(Recorder.StartRecord(102));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.StopRecord());
    SKYWALKER_TEST_ASSERT_FALSE(Recorder.IsRecording());

    std::error_code EC;
    std::filesystem::remove_all(ReplayDir, EC);

    return true;
}

bool TestReplayPlayerState()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    const std::filesystem::path ReplayDir = std::filesystem::temp_directory_path() / "skywalker_replay_test";

    SSFModule_ReplayRecorder Recorder(Context, Errors);
    Recorder.SetReplayDirectory(ReplayDir.string());
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.StartRecord(201));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.RecordEvent("Frame=1"));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.StopRecord());

    SSFModule_ReplayPlayer Player(Context, Errors);
    Player.SetReplayDirectory(ReplayDir.string());

    SKYWALKER_TEST_ASSERT_FALSE(Player.IsReplaying());
    SKYWALKER_TEST_ASSERT_TRUE(Player.StartReplay(201));
    SKYWALKER_TEST_ASSERT_EQ(Player.GetLoadedEventCount(), static_cast<SFUInt64>(1));
    SKYWALKER_TEST_ASSERT_TRUE(Player.BuildStats().find("ReplayFormatVersion=2") != SFString::npos);
    SKYWALKER_TEST_ASSERT_TRUE(Player.IsReplaying());
    SKYWALKER_TEST_ASSERT_FALSE(Player.StartReplay(202));
    SKYWALKER_TEST_ASSERT_TRUE(Player.StopReplay());
    SKYWALKER_TEST_ASSERT_FALSE(Player.IsReplaying());

    std::error_code EC;
    std::filesystem::remove_all(ReplayDir, EC);

    return true;
}

bool TestReplayPlayerChecksumMismatch()
{
    SFModuleContext Context;
    SFObjectErrors Errors;
    const std::filesystem::path ReplayDir = std::filesystem::temp_directory_path() / "skywalker_replay_checksum_test";

    SSFModule_ReplayRecorder Recorder(Context, Errors);
    Recorder.SetReplayDirectory(ReplayDir.string());
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.StartRecord(301));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.RecordEvent("Frame=1"));
    SKYWALKER_TEST_ASSERT_TRUE(Recorder.StopRecord());

    const std::filesystem::path ReplayFilePath = ReplayDir / "Session_301.replay";
    std::ifstream InFile(ReplayFilePath, std::ios::in);
    SKYWALKER_TEST_ASSERT_TRUE(InFile.is_open());

    std::vector<SFString> Lines;
    SFString Line;
    while (std::getline(InFile, Line))
    {
        Lines.push_back(Line);
    }
    InFile.close();

    SKYWALKER_TEST_ASSERT_TRUE(Lines.size() >= 5);
    Lines[4] = "Checksum=1";

    std::ofstream OutFile(ReplayFilePath, std::ios::trunc);
    SKYWALKER_TEST_ASSERT_TRUE(OutFile.is_open());
    for (const auto &CurrentLine : Lines)
    {
        OutFile << CurrentLine << "\n";
    }
    OutFile.close();

    SSFModule_ReplayPlayer Player(Context, Errors);
    Player.SetReplayDirectory(ReplayDir.string());
    SKYWALKER_TEST_ASSERT_FALSE(Player.StartReplay(301));

    std::error_code EC;
    std::filesystem::remove_all(ReplayDir, EC);

    return true;
}

bool TestAdminCommandReplayGateway()
{
    auto &Gateway = SSFGameplayServiceGateway::Instance();
    Gateway.RegisterReplayStartRecord([](SFUInt64 SessionId)
                                      { return SessionId == 888; });
    Gateway.RegisterReplayStopRecord([]()
                                     { return true; });
    Gateway.RegisterReplayStartPlay([](SFUInt64 SessionId)
                                    { return SessionId == 888; });
    Gateway.RegisterReplayStopPlay([]()
                                   { return true; });
    Gateway.RegisterReplayRecordStats([]()
                                      { return SFString("ReplayRecordStats=Test"); });
    Gateway.RegisterReplayPlayStats([]()
                                    { return SFString("ReplayPlayStats=Test"); });
    Gateway.RegisterAISetStrategy([](const SFString &StrategyName)
                                  { return StrategyName == "strict" || StrategyName == "balanced" || StrategyName == "relaxed"; });
    Gateway.RegisterAIGetStats([]()
                               { return SFString("AIStrategy=strict"); });
    Gateway.RegisterAIGetStrategies([]()
                                    { return SFString("AIStrategies=strict|balanced|relaxed"); });

    SFModuleContext Context;
    SFObjectErrors Errors;
    SSFModule_AdminCommand AdminCommand(Context, Errors);
    AdminCommand.Init(Errors);

    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("start_replay_record 888"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("stop_replay_record"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("start_replay 888"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("stop_replay"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("show_replay_stats"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("set_ai_strategy strict"));
    SKYWALKER_TEST_ASSERT_FALSE(AdminCommand.ExecuteCommand("set_ai_strategy unknown"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("show_ai_stats"));
    SKYWALKER_TEST_ASSERT_TRUE(AdminCommand.ExecuteCommand("show_ai_strategies"));

    AdminCommand.Destroy(Errors);
    Gateway.RegisterReplayStartRecord(nullptr);
    Gateway.RegisterReplayStopRecord(nullptr);
    Gateway.RegisterReplayStartPlay(nullptr);
    Gateway.RegisterReplayStopPlay(nullptr);
    Gateway.RegisterReplayRecordStats(nullptr);
    Gateway.RegisterReplayPlayStats(nullptr);
    Gateway.RegisterAISetStrategy(nullptr);
    Gateway.RegisterAIGetStats(nullptr);
    Gateway.RegisterAIGetStrategies(nullptr);

    return true;
}

SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkCodecRoundTrip, TestNetworkCodecRoundTrip)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkCodecInvalidMagic, TestNetworkCodecInvalidMagic)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkRouterDispatch, TestNetworkRouterDispatch)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestNetworkRouterUnhandled, TestNetworkRouterUnhandled)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestBlacklistStoreSaveLoad, TestBlacklistStoreSaveLoad)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestLoginPayloadCodecRoundTrip, TestLoginPayloadCodecRoundTrip)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestLoginPayloadCodecInvalid, TestLoginPayloadCodecInvalid)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestWorldManagerEnterLeave, TestWorldManagerEnterLeave)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestWorldManagerSwitchWorld, TestWorldManagerSwitchWorld)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestWorldManagerInvalidInput, TestWorldManagerInvalidInput)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestAdminCommandExecute, TestAdminCommandExecute)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestGameplayServiceGatewayCallbacks, TestGameplayServiceGatewayCallbacks)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestAIRuntimeTickBudget, TestAIRuntimeTickBudget)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestReplayRecorderState, TestReplayRecorderState)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestReplayPlayerState, TestReplayPlayerState)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestReplayPlayerChecksumMismatch, TestReplayPlayerChecksumMismatch)
SKYWALKER_TEST_REGISTER(SkywalkerNetwork, TestAdminCommandReplayGateway, TestAdminCommandReplayGateway)
