#include "EnemyMoveStop.h"

#include <cassert>

#include <GameTimer.h>
#include <Definition.h>

#include <Enemy/BaseEnemy.h>

void EnemyMoveStop::update() {
	timer += GameTimer::DeltaTime();
	switch (mode) {
	case StopMode::LOOKAROUND:
	{
		auto&& nowQ = enemy->get_transform().get_quaternion();
		Quaternion rotate = Quaternion::AngleAxis(CVector3::BASIS_Y, PI / 6 * GameTimer::DeltaTime());
		if (std::cos(timer * PI /  2) > 0) {
			enemy->get_transform().set_rotate(rotate * nowQ);
		}
		else {
			enemy->get_transform().set_rotate(rotate.inverse() * nowQ);
		}
		break;
	}
	case StopMode::UPDOWN:
	{
		Vector3 velocity = CVector3::BASIS_Y * 3.0f * GameTimer::DeltaTime();
		if (std::cos(timer * PI) > 0) {
			enemy->get_transform().plus_translate(velocity);
		}
		else {
			enemy->get_transform().plus_translate(-velocity);
		}
		break;
	}
		break;
	case StopMode::NANE:
	default:
		assert(false);
		break;
	}
}

void EnemyMoveStop::set_stop_mode(StopMode mode_) {
	mode = mode_;
}
