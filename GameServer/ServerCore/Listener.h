#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
class ServerService;

/*--------------
	Listener
---------------*/
// Listener는 Server Service의 일부로 동작하는 클래스
class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* 외부에서 사용 */
	bool StartAccept(ServerServiceRef service); // 클라이언트 연결 수락을 시작하는 함수
	void CloseSocket();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override; // IOCP 핸들을 반환하는 함수 (소켓 핸들)
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override; // 이벤트를 처리하는 함수

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET; // 클라이언트 연결을 위한 소켓
	vector<AcceptEvent*> _acceptEvents; // 수락 이벤트 리스트
	ServerServiceRef _service; // 서버 서비스 객체 참조
};

