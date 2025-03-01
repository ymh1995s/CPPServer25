#pragma once

/*----------------
	IocpObject
-----------------*/

// 모든 IOCP 객체들은 이 클래스를 상속받아야 함 
class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	// IOCP에서 사용할 핸들을 반환해야 함 (소켓 핸들 등)
	virtual HANDLE GetHandle() abstract;
	// IOCP 이벤트가 완료되었을 때 호출
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

/*--------------
	IocpCore
---------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	// IOCP 핸들 반환
	HANDLE		GetHandle() { return _iocpHandle; }

	// IOCP 객체 등록 (소켓 또는 파일 핸들 등)
	bool		Register(IocpObjectRef iocpObject);
	// IOCP 이벤트를 가져와 처리 (timeoutMs: 최대 대기 시간)
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle; // IOCP 핸들
};