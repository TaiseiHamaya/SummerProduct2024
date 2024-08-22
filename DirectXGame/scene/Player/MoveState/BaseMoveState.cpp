#include "BaseMoveState.h"

void BaseMoveState::initialize() {
}

void BaseMoveState::input(const XINPUT_STATE& joyState) {
	constexpr float DEADZONE = 0.2f;

	moveStickL = CVector2::ZERO;
	moveStickR = CVector2::ZERO;

	moveStickL.x += joyState.Gamepad.sThumbLX / float((std::numeric_limits<short>::max)());
	moveStickL.y += joyState.Gamepad.sThumbLY / float((std::numeric_limits<short>::max)());
	moveStickR.x += joyState.Gamepad.sThumbRX / float((std::numeric_limits<short>::max)());
	moveStickR.y += joyState.Gamepad.sThumbRY / float((std::numeric_limits<short>::max)());

	moveStickL = moveStickL.normalize_safe(DEADZONE, CVector2::ZERO);
	moveStickR = moveStickR.normalize_safe(DEADZONE, CVector2::ZERO);
}

void BaseMoveState::set_camera(const Camera3D* camera_) {
	camera = camera_;
}
