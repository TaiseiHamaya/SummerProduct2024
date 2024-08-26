#pragma once

#define NOMINMAX

#include <memory>

#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>

#include <Transform3D.h>
#include <Vector3.h>

#include <Collision/BaseCollider.h>

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
	bool is_dead() const;
	Vector3 get_position() const;
	const Transform3D& get_transform() const;
	Transform3D& get_transform();
	const Matrix4x4& world_matrix() const;
	std::weak_ptr<BaseCollider> get_collider() const;

	void set_model(const std::shared_ptr<Model>& model_);
	void set_parent(const GameObject& rhs);

protected:
	bool isDead;
	Transform3D transform;
	WorldTransform hierarchy;
	std::shared_ptr<Model> model;
	std::shared_ptr<BaseCollider> collider;

public:
	static void SetStaticViewProjection(const ViewProjection& viewProjection);

protected:
	static const ViewProjection* defaultViewProjection;
};
