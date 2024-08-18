#include "SideMove.h"

Vector3 SideMove::velocity() const {
	constexpr float MOVESPEED = 3.0f;

	float sign = camera->get_position().x >= 0 ? 1.0f : -1.0f;

	Vector3 baseMove = { 0,moveStickL.y, moveStickL.x * sign };

	return baseMove * MOVESPEED;
}

std::optional<Quaternion> SideMove::quaternion() const {
	return std::nullopt;
}
