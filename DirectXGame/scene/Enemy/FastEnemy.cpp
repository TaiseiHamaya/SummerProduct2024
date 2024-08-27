#include "FastEnemy.h"

void FastEnemy::attack() {
	Vector3 forward = Transform3D::HomogeneousVector(CVector3::BASIS_Z, hierarchy.matWorld_);
	attackFunction(get_position(), forward);
	attackCall.restart(1.0f);
}

std::unique_ptr<BaseEnemy> FastEnemy::Create() {
	return std::make_unique<FastEnemy>();
}
