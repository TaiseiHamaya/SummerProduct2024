#include "VerticalMove.h"

void VerticalMove::initialize() {
	BaseMoveState::initialize();
}

Vector3 VerticalMove::velocity() const {
	constexpr float DEADZONE = 0.2f;
	constexpr float MOVESPEED = 3.0f;

	// input
	Vector3 moveStickL;
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState)) {
		moveStickL.x += float(joyState.Gamepad.sThumbLX) / (std::numeric_limits<short>::max)();
		moveStickL.z += float(joyState.Gamepad.sThumbLY) / (std::numeric_limits<short>::max)();
	}

	// デッドゾーン以下の場合、入力なしとする
	if (moveStickL.length() <= DEADZONE) {
		return CVector3::ZERO;
	}

	moveStickL = moveStickL.normalize();

	return moveStickL * MOVESPEED;
}

void VerticalMove::set_field(const RailField* const field_) {
	field = field_;
}
