#include "BaseMoveState.h"

void BaseMoveState::initialize() {
	gameObject = nullptr;
	input = Input::GetInstance();
}

void BaseMoveState::set_target(const GameObject* target) {
	gameObject = target;
}
