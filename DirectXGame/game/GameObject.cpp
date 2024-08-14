#include "GameObject.h"

void GameObject::initialize() {
	hierarchy.Initialize();
	model.reset();
}

void GameObject::update() {
}

void GameObject::begin_rendering() {
	hierarchy.UpdateMatrix(transform);
}

void GameObject::draw([[maybe_unused]] const ViewProjection& viewProjection) const {
	if (!model.expired())
		model.lock()->Draw(hierarchy, viewProjection);
}

Vector3 GameObject::get_position() const {
	return Transform3D::ExtractPosition(hierarchy.matWorld_);
}

void GameObject::set_parent(const GameObject& rhs) {
	hierarchy.parent_ = &rhs.hierarchy;
}
