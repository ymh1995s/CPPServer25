#pragma once
#include <functional>

/*---------
	Job
----------*/

// 함수 객체 선언, ()로 제공되면 펑터인가 보다.
using CallbackType = std::function<void()>;

class Job
{
public:
	Job(CallbackType&& callback) : _callback(std::move(callback))
	{
	}

	template<typename T, typename Ret, typename... Args>
	Job(shared_ptr<T> owner, Ret(T::* memFunc)(Args...), Args&&... args)
	{
		_callback = [owner, memFunc, args...]()
			{
				(owner.get()->*memFunc)(args...);
			};
	}

	void Execute()
	{
		_callback();
	}

private:
	CallbackType _callback;
};

