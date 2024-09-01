#include "ClearScene.h"

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include <TextureManager.h>

#include "GameTimer.h"
#include "SceneManager.h"
#include "Definition.h"

#include "scenes/Title/TitleScene.h"

void ClearScene::initialize() {
	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();

	auto textureManager = TextureManager::GetInstance();

	std::uint32_t whiteHandle = textureManager->Load("sprite/white.png");
	whiteSprite = std::unique_ptr<Sprite>(Sprite::Create(whiteHandle, { 0,0 }));
	std::uint32_t clearHandle = textureManager->Load("sprite/clear/clear.png");
	clearSprite= std::unique_ptr<Sprite>(Sprite::Create(clearHandle, Vector2{ WinApp::kWindowWidth,WinApp::kWindowHeight - 100 } / 2));
	clearSprite->SetAnchorPoint({ 0.5f,0.5f });
	std::uint32_t startSprite = textureManager->Load("sprite/button.png");
	startButton = std::unique_ptr<Sprite>(Sprite::Create(startSprite, Vector2{ WinApp::kWindowWidth / 2,500 }));
	startButton->SetAnchorPoint({ 0.5f,0.5f });

	timer = 0;
	isPrePressButtonA = false;
	isPressButtonA = false;
}

void ClearScene::begin() {
	timer += GameTimer::DeltaTime();
	isPrePressButtonA = isPressButtonA;
	isPressButtonA = false;
}

void ClearScene::update() {
	XINPUT_STATE joyState;
	bool inputResult = input->GetJoystickState(0, joyState);

	if (inputResult) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			isPressButtonA = true;
		}
	}

	if (!isPressButtonA && isPrePressButtonA) {
		SceneManager::GetInstance().set_next_scene(
			std::make_unique<TitleScene>()
		);
	}
	float buttonScale = std::sin(timer * PI) * 0.2f + 1.0f;
	startButton->SetSize(Vector2{ 256,256 } *buttonScale);
}

void ClearScene::begin_rendering() {
}

void ClearScene::after_update() {
}

void ClearScene::draw() const {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	whiteSprite->Draw();
	clearSprite->Draw();
	startButton->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}
