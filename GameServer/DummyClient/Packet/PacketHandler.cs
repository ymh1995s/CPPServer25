﻿using CSharpServerCore;
using Google.Protobuf;

class PacketHandler
{
    public static void S_EnterHandler(PacketSession session, IMessage packet)
    {
        // 더미클라이언트에는 필요 없음
    }

    public static void S_LeaveHandler(PacketSession session, IMessage packet)
    {
        // 더미클라이언트에는 필요 없음
    }

    public static void S_MoveHandler(PacketSession session, IMessage packet)
    {
        // 더미클라이언트에는 필요 없음
    }

    public static void S_SpawnHandler(PacketSession session, IMessage packet)
    {
        // 더미클라이언트에는 필요 없음
    }

    public static void S_DespawnHandler(PacketSession session, IMessage packet)
    {
        // 더미클라이언트에는 필요 없음
    }

    public static void SChatHandler(PacketSession session, IMessage packet)
    {
        // 더미클라이언트에는 필요 없음
    }
}