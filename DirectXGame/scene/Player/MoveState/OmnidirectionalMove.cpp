#include "OmnidirectionalMove.h"

Vector3 OmnidirectionalMove::velocity() const {
	constexpr float MOVESPEED = 3.0f;
	// デッドゾーン以下の場合、入力なしとする
	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	return moveStickL * rotation * MOVESPEED;
}

std::optional<Quaternion> OmnidirectionalMove::quaternion() const {
	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	if (moveStickR != CVector3::ZERO) {
		return Quaternion::LookForward(moveStickR * rotation);
	}
	else if (moveStickL != CVector3::ZERO) {
		return Quaternion::LookForward(moveStickL * rotation);
	}
	else {
		return std::nullopt;
	}
}

void OmnidirectionalMove::set_camera(const Camera3D* camera_) {
	camera = camera_;
}
