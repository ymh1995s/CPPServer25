#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

/*--------------
	Listener
---------------*/

Listener::~Listener()
{
	SocketUtils::Close(_socket);

	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		// TODO

		delete(acceptEvent);
	}
}

// 클라이언트 연결 수락 시작
bool Listener::StartAccept(ServerServiceRef service)
{
	_service = service;
	if (_service == nullptr)
		return false;

	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	// IOCP 등록
	if (_service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;

	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	// 네트워크 주소 바인딩
	if (SocketUtils::Bind(_socket, _service->GetNetAddress()) == false)
		return false;

	// 소켓을 수신 대기 상태로 설정
	if (SocketUtils::Listen(_socket) == false)
		return false;

	// 수락할 연결의 개수만큼 AcceptEvent를 등록
	const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = shared_from_this(); // 자신을 소유자로 설정
		_acceptEvents.push_back(acceptEvent); // 이벤트 리스트에 추가
		RegisterAccept(acceptEvent); // 수락 등록
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

// IOCP 핸들을 반환하는 함수 
HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

// IOCP 이벤트를 처리하는 함수
void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// 이벤트 타입이 Accept여야 한다
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent); // 연결 수락 처리
}

// 새로운 세션 생성 (IOCP에 등록)
void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	SessionRef session = _service->CreateSession(); // Register IOCP

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
	// https://learn.microsoft.com/ko-kr/windows/win32/api/_winsock/
	// ★★★ 실제 연결이 되는 부분 AcceptEx()
	// ★★★ AcceptEx는 비동기 함수로써, 일단 접속을 대기하다가 LPOVERLAPPED 구조체인 acceptEvent에 완료 결과가 담긴다.
	// ★★★ acceptEvent는 IOCP 큐에 담기고, GetQueuedCompletionStatus()에서 꺼내서 워커 스레드가 처리.
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer.WritePos(), 0, 
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// 일단 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
}
// 실제 연결이 확정되고 세션을 등록하는 부분
void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	SessionRef session = acceptEvent->session;

	// 세션의 소켓 업데이트
	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept(acceptEvent); // 실패 시 다시 수락 등록
		return;
	}

	// 클라이언트 주소 가져오기
	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent); // 실패 시 다시 수락 등록
		return;
	}

	// 세션의 네트워크 주소 설정
	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect(); // 연결 처리

	// 새로운 연결을 위한 AcceptEvent 재등록
	RegisterAccept(acceptEvent);
}