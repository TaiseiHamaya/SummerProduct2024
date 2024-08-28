#include "GatlingEnemy.h"

void GatlingEnemy::initialize() {
	BaseEnemy::initialize();
	gatlingCall = {
		std::bind(&GatlingEnemy::reset_gatling, this), 0.08f
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
			attackCall.restart(3.0f);
		}
		Vector3 forward = Transform3D::HomogeneousVector(CVector3::BASIS_Z, hierarchy.matWorld_);
		attackFunction(get_position(), forward);
		gatlingCall.restart(0.16f);
	}
}

std::unique_ptr<BaseEnemy> GatlingEnemy::Create() {
	return std::make_unique<GatlingEnemy>();
}
