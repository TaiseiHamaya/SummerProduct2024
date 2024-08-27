#pragma once

class BaseEnemy;

class BaseEnemyMoveState {
public:
	BaseEnemyMoveState() = default;
	virtual ~BaseEnemyMoveState() = default;

public:
	virtual void update() = 0;
	void set_enemy(BaseEnemy* enemy_);

protected:
	BaseEnemy* enemy;
};
