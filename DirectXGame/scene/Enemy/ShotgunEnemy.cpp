#include "ShotgunEnemy.h"

#include <random>

#include <Easing.h>
#include <Definition.h>

#include <Timeline/GameModeManager.h>

void ShotgunEnemy::initialize() {
	BaseEnemy::initialize();
	shotCount = 0;

	hitpoint = 10;
}

void ShotgunEnemy::attack() {
	++shotCount;
	if (shotCount == 3) {
		shotCount = 0;
		attackCall.restart(1.5f);
	}
	static std::random_device seed_gen;
	static std::mt19937 engine(seed_gen());

	auto nowMode = modeManager->get_mode();
	if (nowMode == GameMode::TRANSITION) {
		nowMode = modeManager->get_transition_data().nextMode;
	}

	Vector3 axis;
	if (nowMode == GameMode::VERTICAL || nowMode == GameMode::OMNIDIRECTIONAL) {
		axis = CVector3::BASIS_Y;
	}
	else if (nowMode == GameMode::SIDE) {
		axis = CVector3::BASIS_X;
	}

	Vector3 forward = Transform3D::HomogeneousVector(CVector3::BASIS_Z, hierarchy.matWorld_);

	for (int i = 0; i < 5; ++i) {
		float angle = Lerp(-20.0f * ToRadian, 20.0f * ToRadian, std::generate_canonical<float, std::numeric_limits<float>::digits>(engine));

		Vector3 direction = forward * Quaternion::AngleAxis(axis, angle);

		attackFunction(get_position(), direction);
	}
}

std::unique_ptr<BaseEnemy> ShotgunEnemy::Create() {
	return std::make_unique<ShotgunEnemy>();
}
