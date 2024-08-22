#include "BaseEnemyState.h"

#include "Enemy/Enemy.h"

BaseEnemyState::BaseEnemyState() = default;

BaseEnemyState::~BaseEnemyState() = default;

void BaseEnemyState::set_enemy(Enemy* enemy_) {
	enemy = enemy_;
}

EnemyStateApproach::EnemyStateApproach() :
	shotTimer(nullptr, 0) {
}

void EnemyStateApproach::update() {
	shotTimer.update();
	if (shotTimer.is_finished()) {
		reset_call();
	}
	if (enemy->get_position().z <= 0) {
		//enemy->set_velocity({ 0, 0.1f, 0 });
		//enemy->set_state(new EnemyStateWithdrawal);
	}
}

void EnemyStateApproach::set_enemy(Enemy* enemy_) {
	BaseEnemyState::set_enemy(enemy_);
	reset_call();
}

void EnemyStateApproach::reset_call() {
	//shotTimer = { std::bind(&Enemy::, enemy), SHOT_INTERVAL };
}

void EnemyStateWithdrawal::update() {
}
