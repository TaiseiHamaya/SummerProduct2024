#include "GazerCamera.h"

#include <random>
#include <limits>

#include <Easing.h>

void GazerCamera::initialize() {
	Camera3D::initialize();

	shakeCall = { []() {} ,0 };
}

void GazerCamera::update() {
	shakeCall.update();
	Vector3 translate = offset * transform.get_quaternion();
	if (!shakeCall.is_finished()) {
		static std::random_device device;
		static std::mt19937 engine{ device() };

		constexpr float SHAKE_LEVEL = 0.3f;
		shake.x = Lerp(-SHAKE_LEVEL, SHAKE_LEVEL, std::generate_canonical<float, std::numeric_limits<float>::digits>(engine));
		shake.y = Lerp(-SHAKE_LEVEL, SHAKE_LEVEL, std::generate_canonical<float, std::numeric_limits<float>::digits>(engine));
		shake.z = Lerp(-SHAKE_LEVEL, SHAKE_LEVEL, std::generate_canonical<float, std::numeric_limits<float>::digits>(engine));
		translate += shake;
	}
	transform.set_translate(translate);
}

void GazerCamera::set_offset(const Vector3& offset_) {
	offset = offset_;
}

const Vector3& GazerCamera::get_offset() const {
	return offset;
}

void GazerCamera::do_shake() {
	shakeCall.restart(0.3f);
}

#ifdef _DEBUG

#include <imgui.h>

void GazerCamera::debug_gui() {
	Camera3D::debug_gui();
	ImGui::DragFloat3("Offset", &offset.x, 0.1f);
}
#endif // _DEBUG
