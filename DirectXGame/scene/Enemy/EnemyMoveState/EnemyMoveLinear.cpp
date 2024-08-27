#include "EnemyMoveLinear.h"

#include <GameTimer.h>

#include <Enemy/BaseEnemy.h>

void EnemyMoveLinear::initialize(const Vector3& direction_, float speed_) {
	direction = direction_;
	speed = speed_;
}

void EnemyMoveLinear::update() {
	enemy->get_transform().plus_translate(direction * speed * GameTimer::DeltaTime());
}
