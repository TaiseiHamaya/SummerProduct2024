#include "BaseMoveState.h"

void BaseMoveState::initialize() {
	p_input = Input::GetInstance();
}

void BaseMoveState::input() {
	constexpr float DEADZONE = 0.2f;

	inputResult = p_input->GetJoystickState(0, joyState);

	moveStickL = CVector3::ZERO;
	moveStickR = CVector3::ZERO;

	if (inputResult) {
		moveStickL.x += float(joyState.Gamepad.sThumbLX) / (std::numeric_limits<short>::max)();
		moveStickL.z += float(joyState.Gamepad.sThumbLY) / (std::numeric_limits<short>::max)();
	}
	if (inputResult) {
		moveStickR.x += float(joyState.Gamepad.sThumbRX) / (std::numeric_limits<short>::max)();
		moveStickR.z += float(joyState.Gamepad.sThumbRY) / (std::numeric_limits<short>::max)();
	}

	moveStickL = moveStickL.normalize_safe(DEADZONE, CVector3::ZERO);
	moveStickR = moveStickR.normalize_safe(DEADZONE, CVector3::ZERO);
}
