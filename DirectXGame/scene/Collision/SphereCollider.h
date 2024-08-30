#pragma once

#include "Collision/BaseCollider.h"

class SphereCollider : public BaseCollider {
public:
	SphereCollider() = default;
	~SphereCollider() = default;

public:
	constexpr std::string type() const override;

public:
	void set_radius(float radius_);
	float get_radius();

private:
	float radius;
};

constexpr std::string SphereCollider::type() const {
	return "Sphere";
}
