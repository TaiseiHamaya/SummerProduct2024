#include "VerticalMove.h"

void VerticalMove::initialize() {
	BaseMoveState::initialize();
}

Vector3 VerticalMove::velocity() const {
	constexpr float DEADZONE = 0.2f;
	constexpr float MOVESPEED = 3.0f;

	// デッドゾーン以下の場合、入力なしとする
	if (moveStickL.length() <= DEADZONE) {
		return CVector3::ZERO;
	}

	return moveStickL.normalize() * MOVESPEED;
}

std::optional<Quaternion> VerticalMove::quaternion() const {
	return std::nullopt;
}
