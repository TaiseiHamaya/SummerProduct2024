#include "Player.h"

#include <cassert>
#include <Input.h>
#include <ViewProjection.h>
#include "Transform3D.h"

#include <Player/MoveState/VerticalMove.h>
#include <Player/MoveState/OmnidirectionalMove.h>

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
	// imgui debug
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 20,250 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 330,165 }, ImGuiCond_Once);
	ImGui::Begin("Player", nullptr, ImGuiWindowFlags_NoSavedSettings);
	transform.debug_gui();
	ImGui::End();
#endif // _DEBUG

	// 入力処理
	moveState->input();

	// 更新処理
	velocity = moveState->velocity();
	transform.plus_translate(velocity * 0.016f);

	auto rotateResult = moveState->quaternion();
	if (rotateResult.has_value()) {
		transform.set_rotate(rotateResult.value());
	}
}

void Player::default_data(const std::shared_ptr<Model>& model_, Vector3&& position) {
	model = model_;
	transform.set_translate(position);
}

void Player::set_state(std::unique_ptr<BaseMoveState>&& moveState_) {
	moveState = std::move(moveState_);
}

void Player::on_collision() {
	// do nothing
}
