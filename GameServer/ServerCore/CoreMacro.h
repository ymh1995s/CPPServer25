#pragma once

#define OUT

/*---------------
	  Lock
---------------*/
// 이걸 사용하면 RAII 패턴 적용이던가? => ㄴㄴ
// Resource Acqusition is initialization
// lock()과 unlock()을 락가드 형식으로 호출해야 RAII가 적용 됨 

// USE_LOCK을 선언함으로써 락을 사용할 예정이라고 알려주고,
// 그 클래스 내부에서 Read / Write Lock을 사용하기 위해 WRITE_LOCK 사용
// 여기서는 커스텀 뮤택스는 사용하지 않고 생 뮤택스를 사용하기 때문에 WRTIE_LOCK만 있는 모습
#define USE_MANY_LOCKS(count)	mutex _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define	WRITE_LOCK_IDX(idx)		lock_guard<mutex> lockGuard_##idx(_locks[idx]);
#define WRITE_LOCK				WRITE_LOCK_IDX(0)

/*---------------
	  Crash
---------------*/

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}