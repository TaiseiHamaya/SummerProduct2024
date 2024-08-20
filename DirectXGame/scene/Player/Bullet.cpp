#include "Bullet.h"

#include <GameTimer.h>

void Bullet::initialize(const Vector3& position, const Vector3& direction_, const std::shared_ptr<Model>& model_) {
	GameObject::initialize();
	model = model_;
	direction = direction_.normalize_safe();
	transform.set_translate(position);
}

void Bullet::update() {
	constexpr float SPEED = 9.0f;

	transform.plus_translate(direction * SPEED * GameTimer::DeltaTime());
}
