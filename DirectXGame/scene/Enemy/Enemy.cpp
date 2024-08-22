#include "Enemy.h"

void Enemy::update() {
}

void Enemy::default_data(const std::shared_ptr<Model>& model_, Vector3&& position) {
	model = model_;
	transform.set_translate(position);
}
