#include "GameObject.h"

const ViewProjection* GameObject::defaultViewProjection = nullptr;

void GameObject::initialize() {
	hierarchy.Initialize();
	model.reset();
}

void GameObject::update() {
}

void GameObject::begin_rendering() {
	hierarchy.UpdateMatrix(transform);
}

void GameObject::draw() const {
	draw(*defaultViewProjection);
}

void GameObject::draw(const ViewProjection& viewProjection) const {
	if (model) {
		model->Draw(hierarchy, viewProjection);
	}
}

bool GameObject::is_dead() const {
	return isDead;
}

Vector3 GameObject::get_position() const {
	return Transform3D::ExtractPosition(hierarchy.matWorld_);
}

const Transform3D& GameObject::get_transform() const {
	return transform;
}

Transform3D& GameObject::get_transform() {
	return transform;
}

const Matrix4x4& GameObject::world_matrix() const {
	return hierarchy.matWorld_;
}

void GameObject::set_model(const std::shared_ptr<Model>& model_) {
	model = model_;
}

void GameObject::set_parent(const GameObject& rhs) {
	hierarchy.parent_ = &rhs.hierarchy;
}

void GameObject::SetStaticViewProjection(const ViewProjection& viewProjection) {
	defaultViewProjection = &viewProjection;
}
