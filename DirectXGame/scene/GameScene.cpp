#include "GameScene.h"

#include "TextureManager.h"
#include "AxisIndicator.h"
#include "Sprite.h"

#include <cassert>

#include "Camera3D.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

GameScene::GameScene() = default;

GameScene::~GameScene() = default;

void GameScene::Initialize() {
	Camera3D::Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// いろいろ
	//textureHandle = TextureManager::Load("uvChecker.png");
	playerModel = std::shared_ptr<Model>(Model::CreateFromOBJ("player", true));
	//skydomeModel = std::shared_ptr<Model>(Model::CreateFromOBJ("skydome", true));

	// 天球
	//skydome = std::make_unique<Skydome>();
	//skydome->initialize(skydomeModel);

	// プレイヤー
	player = std::make_unique<Player>();
	player->initialize();
	player->default_data(playerModel, { 0,0,0 });

	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
	AxisIndicator::GetInstance()->SetTargetViewProjection(viewProjection.get());

	isDebugCameraActive = false;
	debugCamera = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);

	GameObject::SetStaticViewProjection(*viewProjection);
}

void GameScene::Update() {

#ifdef _DEBUG
	Camera3D::DebugGUI();
#endif // _DEBUG

	// 更新処理
	Camera3D::CameraUpdate();
	player->update();

	player->begin_rendering();

#ifdef _DEBUG
	// デバッグカメラ
	if (input_->TriggerKey(DIK_F1)) {
		isDebugCameraActive = !isDebugCameraActive;
	}
	if (isDebugCameraActive) {
		// デバッグカメラ時の更新
		// ImGuiにMOしている際は更新しない
		if (!ImGui::GetIO().WantCaptureMouse) {
			debugCamera->Update();
		}
		//railCamera->update_debug(*debugCamera);
		// ViewProjectionをDebugCameraから更新
		auto&& vp = debugCamera->GetViewProjection();
		viewProjection->matView = vp.matView;
		viewProjection->matProjection = vp.matProjection;
		viewProjection->TransferMatrix();
	}
	else {
		// 通常更新
		//railCamera->update_camera();
		viewProjection->matView = Camera3D::GetViewMatrix();
		viewProjection->matProjection = Camera3D::GetProjectionMatrix();
		viewProjection->TransferMatrix();
	}
#else
	// リリース時は通常更新
	viewProjection->matView = Camera3D::GetViewMatrix();
	viewProjection->matProjection = Camera3D::GetProjectionMatrix();
	viewProjection->TransferMatrix();
#endif
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player->draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
