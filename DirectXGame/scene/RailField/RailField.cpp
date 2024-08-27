#include "RailField.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

void RailField::initialize() {
	GameObject::initialize();
	model.reset(Model::CreateSphere());
	fieldNormal = CVector3::BASIS_Y;
}

void RailField::update() {
	distance = Vector3::DotProduct(fieldNormal, transform.get_translate());
	// imgui debug
#ifdef _DEBUG
	ImGui::Begin("Field", nullptr);
	transform.debug_gui();
	ImGui::End();
#endif // _DEBUG
}

#ifdef _DEBUG
void RailField::debug_draw() const {
	draw();
}
#endif // _DEBUG
