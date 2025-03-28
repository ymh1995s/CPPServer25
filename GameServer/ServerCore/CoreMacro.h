#pragma once

#define OUT

#define NAMESPACE_BEGIN(name)	namespace name {
#define NAMESPACE_END			}

/*---------------
	  Lock
---------------*/
// USE_LOCK : mutex 락을 사용한다고 선언. 보통 헤더에 선언 
// WRITE_LOCK : 락가드를 사용하여 스코프를 벗어날 때 자동으로 해제
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