#include "Player.h"

#include <GameTimer.h>

#include <cassert>
#include <Input.h>
#include <ViewProjection.h>
#include "Transform3D.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Player::Player() = default;

Player::~Player() = default;

void Player::initialize() {
	GameObject::initialize();
	model.reset();

	input = Input::GetInstance();
	velocity = CVector3::ZERO;
}

void Player::update() {
	XINPUT_STATE joyState;
	bool inputResult = input->GetJoystickState(0, joyState);

	// 入力処理
	moveState->input();

	// 更新処理
	velocity = moveState->velocity();
	// velocity加算
	transform.plus_translate(velocity * GameTimer::DeltaTime());

	auto&& rotateResult = moveState->quaternion();
	if (rotateResult.has_value()) {
		// 回転適用
		transform.set_rotate(std::move(rotateResult.value()));
	}

	attackTimer -= GameTimer::DeltaTime();
	if (inputResult && 
		(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) &&
		attackTimer <= 0) {
		attack();
	}
}

void Player::attack() {
	attackTimer = 0.3f;
	if (attackFunction) {
		attackFunction();
	}
}

void Player::default_data(const std::shared_ptr<Model>& model_, Vector3&& position) {
	model = model_;
	transform.set_translate(position);
}

void Player::set_state(std::unique_ptr<BaseMoveState>&& moveState_) {
	moveState = std::move(moveState_);
}

void Player::set_attack_func(const std::function<void(void)>& func) {
	attackFunction = func;
}

void Player::on_collision() {
	// do nothing
}

#ifdef _DEBUG
void Player::debug_gui() {
	// imgui debug
	ImGui::Begin("Player");
	transform.debug_gui();
	ImGui::End();
}
#endif // _DEBUG
