#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "GlobalQueue.h"

/*------------------
	ThreadManager
-------------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	lock_guard<mutex> guard(_lock);

	_threads.push_back(thread([=]()
		{
			// 새 스레드에서 TLS 초기화
			InitTLS();
			
			// 주어진 콜백 함수 실행
			callback();
			
			// 스레드 종료 시 TLS 정리
			DestroyTLS();
		}));

	/* 람다 없이
	
		void ThreadFunction(function<void(void)> callback)
		{
			InitTLS();
			callback();
			DestroyTLS();
		}

		void ThreadManager::Launch(function<void(void)> callback)
		{
			lock_guard<mutex> guard(_lock);
			_threads.push_back(thread(ThreadFunction, callback));
		}
	
	*/
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	// 스레드 ID를 원자적으로 증가시키면서 할당
	static atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}

// 글로벌 큐 : 한 스레드에 일이 몰리는 현상을 받지하기 위해 생김
void ThreadManager::DoGlobalQueueWork()
{
	while (true)
	{
		// 일정 시간동안만 실행 
		uint64 now = ::GetTickCount64();
		if (now > LEndTickCount)
			break;

		// 작업할 시간이 된 큐는 꺼내서 실행
		JobQueueRef jobQueue = GGlobalQueue->Pop();
		if (jobQueue == nullptr)
			break;

		jobQueue->Execute();
	}
}

// 잡타이머 - 예약된 일감 처리
void ThreadManager::DistributeReservedJobs()
{
	const uint64 now = ::GetTickCount64();

	GJobTimer->Distribute(now);
}
