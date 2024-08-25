#include "Bullet.h"

#include <GameTimer.h>

void Bullet::initialize(const Vector3& position, const Vector3& direction_, float speed_) {
	GameObject::initialize();
	direction = direction_.normalize_safe();
	transform.set_translate(position);
	isDead = false;
	lifeTime = 10.0f;
	speed = speed_;
}

void Bullet::update() {
	lifeTime -= GameTimer::DeltaTime();
	if (lifeTime <= 0) {
		isDead = true;
	}

	transform.plus_translate(direction * speed * GameTimer::DeltaTime());
}
