#pragma once

#include <functional>

#include <GameTimer.h>

template<class Type>
class TimedCall {
public:
	TimedCall() = default;
	TimedCall(std::function<Type>&& function_, float time_);
	~TimedCall() = default;

	void update();

	bool is_finished();

	void restart(float time);

private:
	std::function<Type> function;
	float time;
	bool isFinished = false;
};

template<class Type>
inline TimedCall<Type>::TimedCall(std::function<Type>&& function_, float time_) {
	function = function_;
	time = time_;
}

template<class Type>
inline void TimedCall<Type>::update() {
	time -= GameTimer::DeltaTime();
	if (time <= 0) {
		isFinished = true;
		function();
	}
}

template<class Type>
inline bool TimedCall<Type>::is_finished() {
	return isFinished;
}

template<class Type>
inline void TimedCall<Type>::restart(float time_) {
	time = time_;
	isFinished = false;
}
