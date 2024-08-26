#pragma once

#include <GameObject.h>

class Bullet : public GameObject {
public:
	Bullet() = default;
	~Bullet() = default;

private:
	using GameObject::initialize;

public:
	void initialize(const Vector3& position, const Vector3& direction, float speed);
	void update() override;

	void on_collision(const BaseCollider* collider_);

private:
	Vector3 direction;
	float speed;

	float lifeTime;
};

