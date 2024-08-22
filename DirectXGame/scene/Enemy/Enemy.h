#pragma once

#include <GameObject.h>

#include <memory>
#include <functional>

class Enemy : public GameObject {
public: // コンストラクタ
	Enemy() = default;
	~Enemy() = default;

public: // publicメンバ関数
	void update() override;

public:
	void default_data(const std::shared_ptr<Model>& model_, Vector3&& position);
	//void set_state(std::unique_ptr<BaseMoveState>&& moveState_);
	void set_attack_func(const std::function<void(void)>& func);

public:
	void on_collision();

private: // メンバ変数
	//std::unique_ptr<BaseMoveState> moveState;

	float attackTimer;
	std::function<void(void)> attackFunction;
};

