#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"

#include "Camera/GazerCamera.h"

#include "Model.h"

#include <Timeline/GameModeManager.h>
#include <Timeline/GameTimeline.h>

#include "Player/Player.h"
#include "RailField/RailField.h"
#include "Player/Bullet.h"
#include "Enemy/Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:
	void add_player_bullet();
	void add_enemy(std::istringstream& movementFile);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t textureHandle = 0;
	std::shared_ptr<Model> playerModel;
	std::shared_ptr<Model> skydomeModel;
	std::shared_ptr<Model> bulletModel;

	std::unique_ptr<GazerCamera> camera;

	std::unique_ptr<RailField> field;
	std::unique_ptr<GameTimeline> timeline;
	std::unique_ptr<GameModeManager> gameModeManager;

	std::unique_ptr<Player> player;
	std::list<Bullet> playerBullets;
	std::list<Enemy> enemies;
};
