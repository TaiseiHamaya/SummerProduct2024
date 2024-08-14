#pragma once

#include <GameObject.h>

#include <memory>

class Input;
class Sprite;
class Reticle;
class Enemy;

class Player : public GameObject {
public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void initialize() override;
	void update() override;

	void default_data(const std::shared_ptr<Model>& model_, Vector3&& position);

public:
	void on_collision();

private: // メンバ変数
	int attackTimer;

	Input* input;
};