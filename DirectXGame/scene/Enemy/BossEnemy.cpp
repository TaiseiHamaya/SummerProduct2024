#include "BossEnemy.h"

#include <Definition.h>

#include <Timeline/GameModeManager.h>

void BossEnemy::initialize() {
	BaseEnemy::initialize();
	hitpoint = 50;
}

void BossEnemy::update() {
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

	Quaternion rotation = Quaternion::AngleAxis(axis, PI / 3 * GameTimer::DeltaTime());
	Quaternion nowRotation = transform.get_quaternion();

	transform.set_rotate(rotation * nowRotation);
	
	BaseEnemy::update();
}

void BossEnemy::attack() {
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
	for (int i = 0; i < 4; ++i) {
		Quaternion rotation = Quaternion::AngleAxis(axis, PI / 2 * i);
		attackFunction(get_position(), forward * rotation);
	}

	attackCall.restart(0.16f);
}

std::unique_ptr<BaseEnemy> BossEnemy::Create() {
	return std::make_unique<BossEnemy>();
}
