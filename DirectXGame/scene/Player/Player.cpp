#include "Player.h"

#include <cassert>

#include <Input.h>
#include <ViewProjection.h>

#include <GameTimer.h>
#include <Transform3D.h>

#include "Timeline/GameModeManager.h"

#include <Player/MoveState/OmnidirectionalMove.h>
#include <Player/MoveState/SideMove.h>
#include <Player/MoveState/VerticalMove.h>
#include <Player/MoveState/TransitionMove.h>

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

	attackTimer -= GameTimer::DeltaTime();
	
	if (moveState) {
		// 入力処理
		if (inputResult) {
			moveState->input(joyState);
		}

		// 更新処理
		velocity = moveState->velocity();
		// velocity加算
		transform.plus_translate(velocity * GameTimer::DeltaTime());

		auto&& rotateResult = moveState->quaternion();
		if (rotateResult.has_value()) {
			// 回転適用
			transform.set_rotate(
				Quaternion::Slerp(
					std::move(rotateResult.value()),
					transform.get_quaternion(),
					0.7f
				)
			);
		}
	}

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

void Player::default_data(Vector3&& position) {
	transform.set_translate(position);
}

void Player::set_move_state(TransitionData* transitionData) {
	assert(transitionData);
	if (!transitionData->isTransitioning) {
		auto&& temp = std::make_unique<TransitionMove>();
		temp->set_transition_data(transitionData);
		temp->move_now_move_state(std::move(moveState));
		temp->set_player_transform(transform);
		moveState = std::move(temp);
		moveState->initialize();
		moveState->set_camera(camera);
		return;
	}
	switch (transitionData->nextMode) {
	case GameMode::VERTICAL:
		moveState = std::make_unique<VerticalMove>();
		break;
	case GameMode::SIDE:
		moveState = std::make_unique<SideMove>();
		break;
	case GameMode::OMNIDIRECTIONAL:
		moveState = std::make_unique<OmnidirectionalMove>();
		break;
	case GameMode::TRANSITION:
	{
		auto&& temp = std::make_unique<TransitionMove>();
		temp->set_transition_data(transitionData);
		temp->move_now_move_state(std::move(moveState));
		temp->set_player_transform(transform);
		moveState = std::move(temp);
	}
	break;
	case GameMode::NANE:
#ifdef _DEBUG
	case GameMode::DEBUG_:
	case GameMode::EDITOR_:
#endif // _DEBUG
		// do nothing
		break;
	default:
		assert(false);
		break;
	}
	if (moveState) {
		moveState->initialize();
		moveState->set_camera(camera);
	}
}

void Player::set_attack_func(const std::function<void(void)>& func) {
	attackFunction = func;
}

void Player::set_camera(Camera3D* camera_) {
	camera = camera_;
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
