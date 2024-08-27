#include "ToPlayerEnemy.h"

#include <Player/Player.h>

//const Player* ToPlayerEnemy::p_player;

void ToPlayerEnemy::update() {
	BaseEnemy::update();
	transform.look_at(p_player->get_transform());
}

void ToPlayerEnemy::attack() {
	Vector3 position = get_position();
	Vector3 direction = (p_player->get_position() - position).normalize_safe(1e-4f, -CVector3::BASIS_Z);
	attackFunction(position, direction);
	attackCall.restart(1.0f);
}

std::unique_ptr<BaseEnemy> ToPlayerEnemy::Create() {
	return std::make_unique<ToPlayerEnemy>();
}

void ToPlayerEnemy::SetTargetPlayer(const Player* player) {
	p_player = player;
}