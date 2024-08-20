#include "Bullet.h"

#include <GameTimer.h>

void Bullet::initialize(const Vector3& position, const Vector3& direction_, const std::shared_ptr<Model>& model_) {
	GameObject::initialize();
	model = model_;
	direction = direction_.normalize_safe();
	transform.set_translate(position);
	isDead = false;
	lifeTime = 10.0f;
}

void Bullet::update() {
	lifeTime -= GameTimer::DeltaTime();
	if (lifeTime <= 0) {
		isDead = true;
	}

	constexpr float SPEED = 9.0f;

	transform.plus_translate(direction * SPEED * GameTimer::DeltaTime());
}

bool Bullet::is_dead() const {
	return isDead;
}
