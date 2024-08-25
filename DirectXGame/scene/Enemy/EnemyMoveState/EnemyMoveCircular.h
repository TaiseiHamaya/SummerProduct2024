#pragma once

#include <Enemy/EnemyMoveState/BaseEnemyMoveState.h>



class EnemyMoveCircular : public BaseEnemyMoveState {
public:
	EnemyMoveCircular() = default;
	~EnemyMoveCircular() = default;

public:
	void initialize(const Vector3& center_, const Vector3& axis_, float anglePsec_);
	void update() override;

private:
	Vector3 center;
	Vector3 axis;
	float anglePsec;
};
