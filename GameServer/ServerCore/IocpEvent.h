#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv,
	Recv,
	Send
};

/*--------------
	IocpEvent
---------------*/

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void			Init();

public:
	EventType		eventType; // 이벤트 타입 (예: Connect, Disconnect, 등)
	IocpObjectRef	owner; // 해당 이벤트를 소유한 IOCP 객체에 대한 참조
};

/*----------------
	ConnectEvent
-----------------*/

class ConnectEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Connect) 호출
	ConnectEvent() : IocpEvent(EventType::Connect) { }
};

/*--------------------
	DisconnectEvent
----------------------*/

class DisconnectEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Disconnect) 호출
	DisconnectEvent() : IocpEvent(EventType::Disconnect) { }
};

/*----------------
	AcceptEvent
-----------------*/

class AcceptEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Accept) 호출
	AcceptEvent() : IocpEvent(EventType::Accept) { }

public:
	SessionRef	session = nullptr; // 연결된 세션을 나타내는 참조
};

/*----------------
	RecvEvent
-----------------*/

class RecvEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Recv) 호출
	RecvEvent() : IocpEvent(EventType::Recv) { }
};

/*----------------
	SendEvent
-----------------*/

class SendEvent : public IocpEvent
{
public:
	// 기본 생성자는 IocpEvent(EventType::Send) 호출
	SendEvent() : IocpEvent(EventType::Send) { }
	 
	// 송신 버퍼 목록
	vector<SendBufferRef> sendBuffers;
};