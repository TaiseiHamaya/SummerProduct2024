#include "GameScene.h"

#include <functional>

#include "AxisIndicator.h"
#include "Sprite.h"

#include <Camera3D.h>
#include <GameTimer.h>
#include "SceneManager.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/ToPlayerEnemy.h"
#include "scenes/Clear/ClearScene.h"
#include "scenes/Dead/DeadScene.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

GameScene::GameScene() = default;

GameScene::~GameScene() = default;

void GameScene::initialize() {
	load();
	allocate();
}

void GameScene::begin() {
	collisionManager->begin_flame();
}

void GameScene::update() {
	timeline->update();

	gameModeManager->update();

#ifdef _DEBUG
	ImGui::Begin("System");
	camera->debug_gui();
	ImGui::Separator();
	ImGui::Text("%f", GameTimer::DeltaTime());
	ImGui::End();

	ImGui::Begin("Bullets");
	ImGui::Text("Count : %d", playerBullets.size());
	ImGui::End();

	camera->debug_camera();

	gameModeManager->debug_gui();

	timeline->debug_gui();

	player->debug_gui();

	enemyManager->debug_gui();
#endif // _DEBUG

	// 更新処理
	skydome->update();
	field->update();
	camera->update();
	player->update();
	for (auto&& itr = playerBullets.begin(); itr != playerBullets.end(); ++itr) {
		itr->update();
	}
	for (auto&& itr = enemyBullets.begin(); itr != enemyBullets.end(); ++itr) {
		itr->update();
	}
	enemyManager->update();

	playerBullets.remove_if([](const Bullet& bullet) { return bullet.is_dead(); });
	enemyBullets.remove_if([](const Bullet& bullet) { return bullet.is_dead(); });

	if (player->is_dead()) {
		audio_->StopWave(bgmVoiceHandle);
		SceneManager::GetInstance().set_next_scene(
			std::make_unique<DeadScene>()
		);
	}
	if (timeline->is_end_timeline() && enemyManager->enemy_list().empty()) {
		audio_->StopWave(bgmVoiceHandle);
		SceneManager::GetInstance().set_next_scene(
			std::make_unique<ClearScene>()
		);
	}
}

void GameScene::begin_rendering() {
	skydome->begin_rendering();
	field->begin_rendering();
	camera->begin_rendering();
	player->begin_rendering();
	for (auto&& itr = playerBullets.begin(); itr != playerBullets.end(); ++itr) {
		itr->begin_rendering();
	}
	for (auto&& itr = enemyBullets.begin(); itr != enemyBullets.end(); ++itr) {
		itr->begin_rendering();
	}
	enemyManager->begin_rendering();
}

void GameScene::after_update() {
	collisionManager->register_collider("Player", player->get_collider());
	for (auto&& itr = playerBullets.begin(); itr != playerBullets.end(); ++itr) {
		collisionManager->register_collider("PlayerBullet", itr->get_collider());
	}
	for (auto&& itr = enemyBullets.begin(); itr != enemyBullets.end(); ++itr) {
		collisionManager->register_collider("EnemyBullet", itr->get_collider());
	}
	const auto& enemyList = enemyManager->enemy_list();
	for (auto&& itr = enemyList.begin(); itr != enemyList.end(); ++itr) {
		collisionManager->register_collider("Enemy", itr->get()->get_collider());
	}

	collisionManager->update();

	collisionManager->collision("PlayerBullet", "EnemyBullet");
	collisionManager->collision("Player", "EnemyBullet");
	collisionManager->collision("PlayerBullet", "Enemy");
}

void GameScene::draw() const {
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
	for (auto&& itr = playerBullets.begin(); itr != playerBullets.end(); ++itr) {
		itr->draw();
	}
	for (auto&& itr = enemyBullets.begin(); itr != enemyBullets.end(); ++itr) {
		itr->draw();
	}
	enemyManager->draw();
	skydome->draw();

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

	player->draw_ui();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::load() {
	WinApp::GetInstance()->SetSizeChangeMode(WinApp::SizeChangeMode::kNone);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	input_->SetJoystickDeadZone(0, 0, 0);
	audio_ = Audio::GetInstance();

	bgmSoundHandle = audio_->LoadWave("sounds/bgm.wav");

	GameTimer::Initialize();

	// いろいろロード
	playerModel = std::shared_ptr<Model>(Model::CreateFromOBJ("player", true));
	playerBulletModel = std::shared_ptr<Model>(Model::CreateFromOBJ("player/bullet", true));
	enemyBulletModel = std::shared_ptr<Model>(Model::CreateFromOBJ("enemies/bullet", true));
	skydomeModel = std::shared_ptr<Model>(Model::CreateFromOBJ("skydome", true));
}

void GameScene::allocate() {
	bgmVoiceHandle = audio_->PlayWave(bgmSoundHandle, true, 0.18f);

	// 天球
	skydome = std::make_unique<Skydome>();
	skydome->initialize();
	skydome->set_model(skydomeModel);

	// フィールド
	field = std::make_unique<RailField>();
	field->initialize();

	// カメラ
	camera = std::make_unique<GazerCamera>();
	camera->initialize();
	camera->set_parent(*field);
	camera->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45, 0, 0),
		CVector3::ZERO
		});
	camera->set_offset({ 0,0,-30 });

	// プレイヤー
	player = std::make_unique<Player>();
	player->initialize();
	player->default_data({ 0,0,0 });
	player->set_model(playerModel);
	player->set_parent(*field);
	player->set_attack_func(std::bind(&GameScene::add_player_bullet, this));
	player->set_camera(camera.get());

	// GameModeManager
	gameModeManager = std::make_unique<GameModeManager>();
	gameModeManager->initialize();
	gameModeManager->set_camera(camera.get());
	gameModeManager->set_player_func(
		std::bind(&Player::set_move_state, player.get(), std::placeholders::_1)
	);

	// EnemyManager
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->initialize();
	enemyManager->set_field(*field);
	enemyManager->set_attack_function(
		std::bind(&GameScene::add_enemy_bullet, this, std::placeholders::_1, std::placeholders::_2)
	);
	enemyManager->set_game_mode_manager(gameModeManager.get());

	// Timeline
	timeline = std::make_unique<GameTimeline>();
	timeline->set_mode(gameModeManager.get());
	timeline->set_spawn_func(
		std::bind(&EnemyManager::load_pop_file, enemyManager.get(), std::placeholders::_1)
	);
	timeline->set_enemies(&enemyManager->enemy_list());
	timeline->initialize();

	// Collision
	collisionManager = std::make_unique<CollisionManager>();

	// プレイヤー対象のエネミーに関連する初期化
	ToPlayerEnemy::SetTargetPlayer(player.get());

#ifdef _DEBUG
	// 右上の座標軸描画の設定
	AxisIndicator::GetInstance()->SetTargetViewProjection(&camera->get_view_projection());
	AxisIndicator::GetInstance()->SetVisible(true);
#endif // _DEBUG

	// GameObjectのVPを設定
	GameObject::SetStaticViewProjection(camera->get_view_projection());
}

void GameScene::add_player_bullet() {
	playerBullets.emplace_back();
	auto&& newBullet = playerBullets.back();
	Vector3 direction = Transform3D::HomogeneousVector(CVector3::BASIS_Z, player->world_matrix());
	newBullet.initialize(player->get_position(), direction, 30.0f);
	newBullet.set_model(playerBulletModel);
}

void GameScene::add_enemy_bullet(const Vector3& position, const Vector3& direction) {
	enemyBullets.emplace_back();
	auto&& newBullet = enemyBullets.back();
	newBullet.initialize(position, direction, 5.0f);
	newBullet.set_model(enemyBulletModel);

}
