#include "EnemyMoveCircular.h"

#include <GameTimer.h>
#include <Quaternion.h>

#include <Enemy/BaseEnemy.h>

void EnemyMoveCircular::initialize(const Vector3& center_, const Vector3& axis_, float anglePsec_) {
	center = center_;
	axis = axis_;
	anglePsec = anglePsec_;
}

void EnemyMoveCircular::update() {
	Vector3 displacement = enemy->get_transform().get_translate() - center;
	Quaternion rotation = Quaternion::AngleAxis(axis, anglePsec * GameTimer::DeltaTime());
	Vector3 newPosition = displacement * rotation + center;

	enemy->get_transform().set_translate(newPosition);
}
