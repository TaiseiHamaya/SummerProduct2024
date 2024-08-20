#pragma once

#include <GameObject.h>

class Bullet : public GameObject {
public:
	Bullet() = default;
	~Bullet() = default;

private:
	using GameObject::initialize;

public:
	void initialize(const Vector3& position, const Vector3& direction, const std::shared_ptr<Model>& model_);
	void update() override;

public:

private:
	Vector3 direction;
};

