#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"

#include "Camera3D.h"

#include "Model.h"

#include "Player/Player.h"
#include "RailField/RailField.h"

#ifdef _DEBUG
#include "DebugCamera.h"
#endif // _DEBUG

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

	std::unique_ptr<Camera3D> camera;

	std::unique_ptr<Player> player;
	std::unique_ptr<RailField> field;
};
