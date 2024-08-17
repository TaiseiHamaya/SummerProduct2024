#include "OmnidirectionalMove.h"

constexpr float DEADZONE = 0.2f;
constexpr float MOVESPEED = 3.0f;

Vector3 OmnidirectionalMove::velocity() const {
	// デッドゾーン以下の場合、入力なしとする
	if (moveStickL.length() <= DEADZONE) {
		return CVector3::ZERO;
	}

	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	return moveStickL.normalize() * rotation * MOVESPEED;
}

std::optional<Quaternion> OmnidirectionalMove::quaternion() const {
	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	if (moveStickR.length() >= DEADZONE) {
		return Quaternion::LookForward(moveStickR.normalize() * rotation);
	}
	else if (moveStickL.length() >= DEADZONE) {
		return Quaternion::LookForward(moveStickL.normalize() * rotation);
	}
	else {
		return std::nullopt;
	}
}

void OmnidirectionalMove::set_camera(const Camera3D* camera_) {
	camera = camera_;
}
