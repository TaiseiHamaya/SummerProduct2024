#include "VerticalMove.h"

Vector3 VerticalMove::velocity() const {
	constexpr float MOVESPEED = 3.0f;
	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	Vector3 baseMove = { moveStickL.x, 0, moveStickL.y };

	return  baseMove * rotation * MOVESPEED;
}

std::optional<Quaternion> VerticalMove::quaternion() const {
	return std::nullopt;
}
