#include "GameScene.h"

#include <cassert>

#include "TextureManager.h"
#include "AxisIndicator.h"
#include "Sprite.h"

#include <Camera3D.h>

#include <Player/MoveState/OmnidirectionalMove.h>
#include <Player/MoveState/SideMove.h>
#include <Player/MoveState/VerticalMove.h>

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

GameScene::GameScene() = default;

GameScene::~GameScene() = default;

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	input_->SetJoystickDeadZone(0, 0, 0);
	audio_ = Audio::GetInstance();

	// いろいろ
	//textureHandle = TextureManager::Load("uvChecker.png");
	playerModel = std::shared_ptr<Model>(Model::CreateFromOBJ("player", true));
	//skydomeModel = std::shared_ptr<Model>(Model::CreateFromOBJ("skydome", true));

	field = std::make_unique<RailField>();
	field->initialize();

	camera = std::make_unique<GazerCamera>();
	camera->initialize();
	camera->set_parent(*field);
	camera->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45, 0, 0),
		{ 0, 10, -10 }
		});
	camera->set_offset({ 0,0,-10 });

	// 天球
	//skydome = std::make_unique<Skydome>();
	//skydome->initialize(skydomeModel);

	// プレイヤー
	player = std::make_unique<Player>();
	player->initialize();
	player->default_data(playerModel, { 0,0,0 });
	player->set_parent(*field);

	auto tempMoveState = std::make_unique<VerticalMove>();
	tempMoveState->initialize();
	tempMoveState->set_camera(camera.get());

	player->set_state(std::move(tempMoveState));

	AxisIndicator::GetInstance()->SetTargetViewProjection(&camera->get_view_projection());

#ifdef _DEBUG
	AxisIndicator::GetInstance()->SetVisible(true);
#endif // _DEBUG

	GameObject::SetStaticViewProjection(camera->get_view_projection());
}

void GameScene::Update() {

#ifdef _DEBUG
	ImGui::SetNextWindowSize(ImVec2{ 330,190 }, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2{ 20, 20 }, ImGuiCond_Once);
	ImGui::Begin("3DCamera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	camera->debug_gui();
	ImGui::End();
	camera->debug_camera();
#endif // _DEBUG

	// 更新処理
	field->update();
	camera->update();
	player->update();

	field->begin_rendering();
	camera->begin_rendering();
	player->begin_rendering();
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

#ifdef _DEBUG
	field->debug_draw();
#endif // _DEBUG

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
