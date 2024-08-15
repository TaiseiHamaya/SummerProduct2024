#pragma once

#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>

#include <Transform3D.h>
#include <Vector3.h>

#include <memory>

class GameObject {
public:
	virtual void initialize();
	virtual void update();
	void begin_rendering();
	virtual void draw() const;
	virtual void draw(const ViewProjection& viewProjection) const;

public:
	Vector3 get_position() const;

	void set_parent(const GameObject& rhs);

protected:
	Transform3D transform;
	WorldTransform hierarchy;
	std::weak_ptr<Model> model;

public:
	static void SetStaticViewProjection(const ViewProjection& viewProjection);

private:
	static const ViewProjection* defaultViewProjection;
};
