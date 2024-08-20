#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"

#include "Camera/GazerCamera.h"

#include "Model.h"

#include "Player/Player.h"
#include "RailField/RailField.h"
#include "Player/Bullet.h"

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

	std::unique_ptr<Player> player;
	std::unique_ptr<RailField> field;
	std::list<Bullet> bullets;
};
