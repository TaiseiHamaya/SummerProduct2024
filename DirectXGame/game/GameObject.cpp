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

Vector3 GameObject::get_position() const {
	return Transform3D::ExtractPosition(hierarchy.matWorld_);
}

const Transform3D& GameObject::get_transform() const {
	return transform;
}

Transform3D& GameObject::get_transform() {
	return transform;
}

void GameObject::set_parent(const GameObject& rhs) {
	hierarchy.parent_ = &rhs.hierarchy;
}

void GameObject::SetStaticViewProjection(const ViewProjection& viewProjection) {
	defaultViewProjection = &viewProjection;
}
