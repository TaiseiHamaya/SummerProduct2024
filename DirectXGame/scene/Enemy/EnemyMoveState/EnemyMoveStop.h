#pragma once

#include <Enemy/EnemyMoveState/BaseEnemyMoveState.h>

class EnemyMoveStop : public BaseEnemyMoveState {
public:
	EnemyMoveStop() = default;
	~EnemyMoveStop() = default;

public:
	void update() override;
};
