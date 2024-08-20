#pragma once

#include <GameObject.h>

#include <memory>
#include <functional>

#include <Player/MoveState/BaseMoveState.h>

class Input;

class Player : public GameObject {
public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void initialize() override;
	void update() override;

	void attack();

public:
	void default_data(const std::shared_ptr<Model>& model_, Vector3&& position);
	void set_state(std::unique_ptr<BaseMoveState>&& moveState_);
	void set_attack_func(const std::function<void(void)>& func);

public:
	void on_collision();

private: // メンバ変数
	Input* input = nullptr;

	std::unique_ptr<BaseMoveState> moveState;
	Vector3 velocity;

	float attackTimer;
	std::function<void(void)> attackFunction;
};