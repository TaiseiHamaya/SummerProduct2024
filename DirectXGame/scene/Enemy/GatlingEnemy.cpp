#include "GatlingEnemy.h"

#include <random>

#include <Easing.h>
#include <Definition.h>

#include <Timeline/GameModeManager.h>

void GatlingEnemy::initialize() {
	BaseEnemy::initialize();
	gatlingCall = {
		std::bind(&GatlingEnemy::reset_gatling, this), 0
	};
	shotCount = 0;
}

void GatlingEnemy::attack() {
	gatlingCall.update();
}

void GatlingEnemy::reset_gatling() {
	if (gatlingCall.is_finished()) {
		++shotCount;
		if (shotCount == 15) {
			shotCount = 0;
			attackCall.restart(2.0f);
		}
		static std::random_device seed_gen;
		static std::mt19937 engine(seed_gen());

		Vector3 forward = Transform3D::HomogeneousVector(CVector3::BASIS_Z, hierarchy.matWorld_);
		float angle = Lerp(-10.0f * ToRadian, 10.0f * ToRadian, std::generate_canonical<float, std::numeric_limits<float>::digits>(engine));

		auto nowMode = modeManager->get_mode();
		if (nowMode == GameMode::TRANSITION) {
			nowMode = modeManager->get_transition_data().nextMode;
		}

		if (nowMode == GameMode::VERTICAL || nowMode == GameMode::OMNIDIRECTIONAL) {
			forward = forward * Quaternion::AngleAxis(CVector3::BASIS_Y, angle);
		}
		else if (nowMode == GameMode::SIDE) {
			forward = forward * Quaternion::AngleAxis(CVector3::BASIS_X, angle);
		}

		attackFunction(get_position(), forward);
		gatlingCall.restart(0.16f);
	}
}

std::unique_ptr<BaseEnemy> GatlingEnemy::Create() {
	return std::make_unique<GatlingEnemy>();
}
