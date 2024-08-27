#pragma once

#include <GameObject.h>

#include <functional>
#include <sstream>
#include <memory>
#include <TimedCall.h>

#include <Enemy/EnemyMoveState/BaseEnemyMoveState.h>

class GameModeManager;

class BaseEnemy : public GameObject {
public: // コンストラクタ
	BaseEnemy() = default;
	virtual ~BaseEnemy() = default;

	// コピー禁止、ムーブ許可
	BaseEnemy(const BaseEnemy&) = delete;
	BaseEnemy& operator=(const BaseEnemy&) = delete;
	BaseEnemy(BaseEnemy&&) = default;
	BaseEnemy& operator=(BaseEnemy&&) = default;

public: // publicメンバ関数
	void initialize() override;
	void update() override;

	void load_move(const std::string& fileName);

protected:
	virtual void attack();

private:
	void next_command();
	void create_move(std::istringstream& command);

public:
	//void set_state();
	void set_attack_func(const std::function<void(const Vector3&, const Vector3&)>& func);
	void set_game_mode_manager(const GameModeManager* manager);

public:
	void on_collision(const BaseCollider* collider_);

public:
	static std::unique_ptr<BaseEnemy> Create();

protected: // メンバ変数
	bool isAttack;
	int hitpoint;
	
	std::unique_ptr<BaseEnemyMoveState> moveState;
	const GameModeManager* modeManager;

	std::function<void(const Vector3&, const Vector3&)> attackFunction;
	TimedCall<void(void)> attackCall;

	std::stringstream moveCommand;
	TimedCall<void(void)> moveCall;
};

