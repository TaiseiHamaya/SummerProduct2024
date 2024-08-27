#pragma once

#include <Enemy/EnemyMoveState/BaseEnemyMoveState.h>

enum class StopMode {
	NANE,
	LOOKAROUND,
	UPDOWN,
};

class EnemyMoveStop : public BaseEnemyMoveState {
public:
	EnemyMoveStop() = default;
	~EnemyMoveStop() = default;

public:
	void update() override;

public:
	void set_stop_mode(StopMode mode_);

private:
	float timer = 0;
	StopMode  mode;
};
