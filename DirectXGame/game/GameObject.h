#pragma once

#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>

#include <Transform3D.h>
#include <Vector3.h>

#include <memory>

class GameObject {
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	// コピー禁止、ムーブ許可
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;

public:
	virtual void initialize();
	virtual void update();
	virtual void begin_rendering();
	virtual void draw() const;
	virtual void draw(const ViewProjection& viewProjection) const;

public:
	Vector3 get_position() const;
	const Transform3D& get_transform() const;
	Transform3D& get_transform();


	void set_parent(const GameObject& rhs);

protected:
	Transform3D transform;
	WorldTransform hierarchy;
	std::shared_ptr<Model> model;

public:
	static void SetStaticViewProjection(const ViewProjection& viewProjection);

protected:
	static const ViewProjection* defaultViewProjection;
};
