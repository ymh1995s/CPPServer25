#pragma once

#include <mutex>
#include <atomic>

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// 세션 데이터 유지를 위한 레퍼런스 카운트
using ServerServiceRef = std::shared_ptr<class ServerService>;
using PacketSessionRef = std::shared_ptr<class PacketSession>;
using IocpObjectRef = std::shared_ptr<class IocpObject>;
using SessionRef = std::shared_ptr<class Session>;
using SendBufferRef = std::shared_ptr<class SendBuffer>;
using IocpCoreRef = std::shared_ptr<class IocpCore>;
using ListenerRef = std::shared_ptr<class Listener>;
using JobQueueRef = std::shared_ptr<class JobQueue>;
using JobRef = std::shared_ptr<class Job>;

#define size16(val)		static_cast<int16>(sizeof(val))
#define size32(val)		static_cast<int32>(sizeof(val))
#define len16(arr)		static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)		static_cast<int32>(sizeof(arr)/sizeof(arr[0]))