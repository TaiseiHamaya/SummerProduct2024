#include "GazerCamera.h"

void GazerCamera::update() {
	transform.set_translate(offset * transform.get_quaternion().inverse());
}

void GazerCamera::set_offset(const Vector3& offset_) {
	offset = offset_;
}

const Vector3& GazerCamera::get_offset() const {
	return offset;
}

#ifdef _DEBUG

#include <imgui.h>

void GazerCamera::debug_gui() {
	Camera3D::debug_gui();
	ImGui::DragFloat3("Offset", &offset.x, 0.1f);
}
#endif // _DEBUG
