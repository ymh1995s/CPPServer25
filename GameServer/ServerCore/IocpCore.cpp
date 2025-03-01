#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

/*--------------
	IocpCore
---------------*/

// IOCP 핸들 생성
IocpCore::IocpCore()
{
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(_iocpHandle);
}

// IOCP 객체(소켓 등)를 IOCP에 등록
bool IocpCore::Register(IocpObjectRef iocpObject)
{
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*key*/0, 0);
}

/*
	IOCP 이벤트 처리:
	- `GetQueuedCompletionStatus`를 호출하여 완료된 IO 이벤트를 가져온다.
	- 이벤트가 있으면 해당 `IocpEvent` 객체를 찾고, 연결된 `IocpObject`의 `Dispatch` 함수를 호출하여 이벤트 처리.
	- 에러 발생 시 에러 코드에 따라 처리 (예: 타임아웃 발생 시 false 반환).

	IOCP 이벤트 : connect diconnect recv send 등
*/
bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;	
	IocpEvent* iocpEvent = nullptr;

	// 무한 대기 하다 완료된 IO 이벤트 가져오기
	// OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent) 인자로 완료된 iocp 이벤트를 가져온다.
	if (::GetQueuedCompletionStatus(_iocpHandle, OUT &numOfBytes, OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		// 이벤트를 소유한 객체를 찾아 Dispatch 호출
		IocpObjectRef iocpObject = iocpEvent->owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : 로그 찍기
			IocpObjectRef iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
