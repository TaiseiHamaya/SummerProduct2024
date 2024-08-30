#include "ShotgunEnemy.h"

#include <random>

#include <Easing.h>
#include <Definition.h>

#include "Timeline/GameModeManager.h"

void ShotgunEnemy::initialize() {
	BaseEnemy::initialize();
	shotCount = 0;

	timer = 0;
	hitpoint = 10;
	reset_bullet();
}

void ShotgunEnemy::attack() {
	Vector3 forward = Transform3D::HomogeneousVector(CVector3::BASIS_Z, hierarchy.matWorld_);
	for (auto& info : generateBulletsInfo) {
		if (info.isShot) {
			continue;
		}
		if (info.shotTime < timer) {

			Vector3 direction = forward * info.rotation;
			attackFunction(get_position(), direction);

			info.isShot = true;
			++shotCount;
		}
	}

	timer += GameTimer::DeltaTime();
	if (timer >= 0.2f) {
		timer = 0;
		shotCount = 0;
		reset_bullet();
		attackCall.restart(1.5f);
	}
}

void ShotgunEnemy::reset_bullet() {
	static std::random_device seed;
	static std::mt19937 engine(seed());

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

	std::array<float, NUM_BULLETS> shotTime;
	for (auto& itr : shotTime) {
		itr = Lerp(0.0f, 0.2f, std::generate_canonical<float, std::numeric_limits<float>::digits>(engine));
	}

	for (int i = 0; auto & itr : shotTime) {
		float angle = Lerp(-45.0f * ToRadian, 45.0f * ToRadian, (float)i / (NUM_BULLETS - 1));
		float randamize = Lerp(-2.0f * ToRadian, 2.0f * ToRadian, std::generate_canonical<float, std::numeric_limits<float>::digits>(engine));
		Quaternion rotation = Quaternion::AngleAxis(axis, angle + randamize);

		generateBulletsInfo[i] = { rotation, itr, false };
		++i;
	}
}

std::unique_ptr<BaseEnemy> ShotgunEnemy::Create() {
	return std::make_unique<ShotgunEnemy>();
}
