#pragma once

#include <functional>

#include <GameTimer.h>

template<class Type>
class TimedCall {
public:
	TimedCall(std::function<Type>&& function_, float time_);

	void update();

	bool is_finished();

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
