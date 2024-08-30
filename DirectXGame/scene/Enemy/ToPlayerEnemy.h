#pragma once

#include "Enemy/BaseEnemy.h"

class Player;

class ToPlayerEnemy : public BaseEnemy {
public:
	ToPlayerEnemy() = default;
	~ToPlayerEnemy() = default;

private:
	void update() override;
	void attack() override;

public:
	static std::unique_ptr<BaseEnemy> Create();
	static void SetTargetPlayer(const Player* player);

private:
	inline static const Player* p_player = nullptr;
};
