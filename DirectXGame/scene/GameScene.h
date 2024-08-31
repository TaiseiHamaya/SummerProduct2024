#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <BaseScene.h>

#include "Camera/GazerCamera.h"

#include "Model.h"

#include "Timeline/GameModeManager.h"
#include "Timeline/GameTimeline.h"
#include "Collision/CollisionManager.h"
#include "Skydome/Skydome.h"
#include "Player/Player.h"
#include "RailField/RailField.h"
#include "Player/Bullet.h"
#include "Enemy/EnemyManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

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
	void initialize() override;
	
	void begin();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void update() override;

	void begin_rendering() override;

	void after_update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void draw() const override;

	void load();
	void allocate();

public:
	void add_player_bullet();
	void add_enemy_bullet(const Vector3& position, const Vector3& direction);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t textureHandle = 0;
	std::shared_ptr<Model> skydomeModel;
	std::shared_ptr<Model> playerModel;
	std::shared_ptr<Model> playerBulletModel;
	std::shared_ptr<Model> enemyBulletModel;

	std::unique_ptr<GazerCamera> camera;

	std::unique_ptr<RailField> field;
	std::unique_ptr<Skydome> skydome;
	std::unique_ptr<GameTimeline> timeline;
	std::unique_ptr<GameModeManager> gameModeManager;
	std::unique_ptr<CollisionManager> collisionManager;

	std::unique_ptr<Player> player;
	std::list<Bullet> playerBullets;
	std::unique_ptr<EnemyManager> enemyManager;
	std::list<Bullet> enemyBullets;

	std::uint32_t bgmSoundHandle;
};
