#include "BaseMoveState.h"

void BaseMoveState::initialize() {
	p_input = Input::GetInstance();
}

void BaseMoveState::input() {
	constexpr float DEADZONE = 0.2f;

	inputResult = p_input->GetJoystickState(0, joyState);

	moveStickL = CVector2::ZERO;
	moveStickR = CVector2::ZERO;

	if (inputResult) {
		moveStickL.x += float(joyState.Gamepad.sThumbLX) / (std::numeric_limits<short>::max)();
		moveStickL.y += float(joyState.Gamepad.sThumbLY) / (std::numeric_limits<short>::max)();
		moveStickR.x += float(joyState.Gamepad.sThumbRX) / (std::numeric_limits<short>::max)();
		moveStickR.y += float(joyState.Gamepad.sThumbRY) / (std::numeric_limits<short>::max)();
	}

	moveStickL = moveStickL.normalize_safe(DEADZONE, CVector2::ZERO);
	moveStickR = moveStickR.normalize_safe(DEADZONE, CVector2::ZERO);
}

void BaseMoveState::set_camera(const Camera3D* camera_) {
	camera = camera_;
}
