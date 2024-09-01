#include "Bullet.h"

#include <GameTimer.h>

#include "Collision/SphereCollider.h"

void Bullet::initialize(const Vector3& position, const Vector3& direction_, float speed_) {
	GameObject::initialize();
	direction = direction_.normalize_safe();
	transform.set_translate(position + direction_);
	transform.set_rotate(Quaternion::LookForward(direction_));
	isDead = false;
	lifeTime = 10.0f;
	speed = speed_;

	auto tempCollider = std::make_shared<SphereCollider>();
	tempCollider->set_matrix(hierarchy.matWorld_);
	tempCollider->set_callback(std::bind(&Bullet::on_collision, this, std::placeholders::_1));
	tempCollider->set_radius(0.5f);
	collider = std::move(tempCollider);
}

void Bullet::update() {
	lifeTime -= GameTimer::DeltaTime();
	if (lifeTime <= 0) {
		isDead = true;
	}

	transform.plus_translate(direction * speed * GameTimer::DeltaTime());
}

void Bullet::on_collision([[maybe_unused]] const BaseCollider* collider_) {
	isDead = true;
	if (particleFunction) {
		particleFunction();
	}
}

void Bullet::set_particle_function(const std::function<void(void)>& function) {
	particleFunction = function;
}
