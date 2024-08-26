#include "BaseCollider.h"

#include <Transform3D.h>

void BaseCollider::update() {
	worldPosition = Transform3D::ExtractPosition(*worldMatrix);
}

Vector3 BaseCollider::position() const {
	return worldPosition;
}

void BaseCollider::callback(const BaseCollider* collision) {
	if (callBackFunction) {
		callBackFunction(collision);
	}
}

void BaseCollider::set_matrix(const Matrix4x4& matrix) {
	worldMatrix = &matrix;
}

void BaseCollider::set_callback(std::function<void(const BaseCollider*)> function) {
	callBackFunction = function;
}
