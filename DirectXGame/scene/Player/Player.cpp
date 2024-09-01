#include "Player.h"

#include <cassert>

#include <Input.h>
#include <ViewProjection.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <Audio.h>
#include <WinApp.h>

#include <GameTimer.h>
#include <Transform3D.h>

#include "Camera/GazerCamera.h"
#include "Timeline/GameModeManager.h"
#include "Collision/SphereCollider.h"
#include "Player/MoveState/OmnidirectionalMove.h"
#include "Player/MoveState/SideMove.h"
#include "Player/MoveState/VerticalMove.h"
#include "Player/MoveState/TransitionMove.h"

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

	auto tempCollider = std::make_shared<SphereCollider>();
	tempCollider->set_matrix(hierarchy.matWorld_);
	tempCollider->set_callback(std::bind(&Player::on_collision, this, std::placeholders::_1));
	tempCollider->set_radius(0.5f);
	collider = std::move(tempCollider);

	auto handle = TextureManager::GetInstance()->Load("player/hitpoint/hitpoint.png");
	for (int i = 0; auto & healthItr : healthData) {
		healthItr.sprite = std::unique_ptr<Sprite>(Sprite::Create(handle, { 0,0 }));
		healthItr.sprite->Initialize();
		healthItr.sprite->SetSize({ 100,100 });
		healthItr.sprite->SetPosition({ 100.0f * i, 620 });
		healthItr.flashCount = 0;
		healthItr.isDraw = true;
		healthItr.flashingCall =
			TimedCall<void(void)>(
				[&healthItr]() {
					healthItr.isDraw ^= 0b1;
					++healthItr.flashCount;
					healthItr.flashingCall.restart(0.02f);
					if (healthItr.flashCount > 12) {
						healthItr.isFlashing = false;
						healthItr.isDraw = false;
					}
				},
				0
			);
		++i;
	}
	invincibleTimer = 0.0f;

	audio = Audio::GetInstance();
	shotSoundHandle = audio->LoadWave("sounds/shot.wav");
	damageSoundHandle = audio->LoadWave("sounds/damaged.wav");

	doneRotationTutorial = false;
	isShowRotationTutorial = false;
	rotationTutorialSpriteHandle = TextureManager::GetInstance()->Load("sprite/UI/tutorial/rotation.png");
	rotationTutorialCall = {
		[&]() { isShowRotationTutorial = false; },
		5
	};
	rotationTutorialSprite = std::unique_ptr<Sprite>(Sprite::Create(rotationTutorialSpriteHandle, { 0,0 }));
	rotationTutorialSprite->SetSize({ 128,128 });
	rotationTutorialSprite->SetAnchorPoint({0.0f,0.5f});
}

void Player::update() {
	XINPUT_STATE joyState;
	bool inputResult = input->GetJoystickState(0, joyState);

	attackTimer -= GameTimer::DeltaTime();
	invincibleTimer -= GameTimer::DeltaTime();

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
					transform.get_quaternion(),
					std::move(rotateResult.value()),
					0.3f
				)
			);
		}
	}

	if (inputResult &&
		(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) &&
		attackTimer <= 0) {
		attack();
	}

	for (auto& healthItr : healthData) {
		if (healthItr.isFlashing) {
			healthItr.flashingCall.update();
		}
	}

	if (doneRotationTutorial) {
		rotationTutorialCall.update();
	}

	Matrix4x4 viewport = Camera3D::MakeViewportMatrix({ 0,0 }, { WinApp::kWindowWidth, WinApp::kWindowHeight }, 0, 1);

	Vector3 playerScreenPosition =
		Transform3D::Homogeneous(
			CVector3::ZERO,
			hierarchy.matWorld_ * defaultViewProjection->matView * defaultViewProjection->matProjection * viewport
		);

	rotationTutorialSprite->SetPosition({ playerScreenPosition.x + 50, playerScreenPosition.y });
}

void Player::draw_ui() const {
	for (const auto& healthItr : healthData) {
		if (healthItr.isDraw) {
			healthItr.sprite->Draw();
		}
	}

	if (isShowRotationTutorial) {
		rotationTutorialSprite->Draw();
	}
}

void Player::attack() {
	attackTimer = 0.08f;
	if (attackFunction) {
		audio->PlayWave(shotSoundHandle, false, 0.05f);
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
		if (!doneRotationTutorial) {
			doneRotationTutorial = true;
			isShowRotationTutorial = true;
		}
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

void Player::set_camera(GazerCamera* camera_) {
	camera = camera_;
	cameraShakeFunction = std::bind(&GazerCamera::do_shake, camera_);
}

void Player::on_collision([[maybe_unused]] const BaseCollider* collider_) {
	if (invincibleTimer >= 0) {
		return;
	}

	--health;
	invincibleTimer = 0.5f;
	if (health >= 0) {
		healthData[health].isFlashing = true;
	}
	cameraShakeFunction();
	audio->PlayWave(damageSoundHandle, false, 0.7f);
}

#ifdef _DEBUG
void Player::debug_gui() {
	// imgui debug
	ImGui::Begin("Player");
	transform.debug_gui();
	ImGui::Separator();
	if (ImGui::Button("Reset health")) {
		health = MAX_HEALTH;
		for (auto& itr : healthData) {
			itr.flashCount = 0;
			itr.isDraw = true;
			itr.isFlashing = false;
		}
	}
	ImGui::Text("Invincible timer : %f", invincibleTimer);
	ImGui::End();
}
#endif // _DEBUG
