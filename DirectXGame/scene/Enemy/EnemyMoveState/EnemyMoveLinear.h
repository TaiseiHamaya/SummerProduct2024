#pragma once

#include "Enemy/EnemyMoveState/BaseEnemyMoveState.h"

#include <Vector3.h>

class EnemyMoveLinear : public BaseEnemyMoveState {
public:
	EnemyMoveLinear() = default;
	~EnemyMoveLinear() = default;

public:
	void initialize(const Vector3& direction_, float speed);
	void update() override;

private:
	Vector3 direction;
	float speed;
};

