#include "GameModeManager.h"

#include <string>
#include <format>

#include <Utility.h>

#include <GameTimer.h>

#include "Camera/GazerCamera.h"

void GameModeManager::initialize() {
#ifdef _DEBUG
	nowMode = GameMode::DEBUG_;
#else
	nowMode = GameMode::NANE;
#endif // _DEBUG
}

void GameModeManager::update() {
	if (nowMode == GameMode::TRANSITION) {
		transitionData.timer += GameTimer::DeltaTime();

		camera->get_transform().set_rotate(
			Quaternion::Slerp(
				transitionData.beginCameraQuaternion,
				transitionData.afterCameraQuaternion,
				std::min(transitionData.parametric(), 1.0f)
			)
		);
		camera->set_offset(
			Vector3::Lerp(
				transitionData.beginOffset,
				transitionData.afterOffset,
				std::min(transitionData.parametric(), 1.0f)
			)
		);

		if (transitionData.transitionTime <= transitionData.timer) {
			playerMoveStateFunc(&transitionData);
			nowMode = transitionData.nextMode;
			transitionData.isTransitioning = false;
			Log(std::format("[GameModeManager] Set GameMode : {}\n", static_cast<int>(nowMode)));
		}
	}
	else if (nowMode == GameMode::VERTICAL || nowMode == GameMode::SIDE || nowMode == GameMode::OMNIDIRECTIONAL) {

	}
#ifdef _DEBUG
	else if (nowMode == GameMode::DEBUG_) {

	}
	else if (nowMode == GameMode::EDITOR_) {

	}
#endif // _DEBUG
}

void GameModeManager::game_mode_command(std::istringstream& command) {
	std::string mode;
	std::string time;
	std::getline(command, mode, ',');
	std::getline(command, time, ',');

	set_next_mode(mode, std::stof(time));
}

void GameModeManager::set_next_mode(const std::string& mode, float time) {
	// 次のゲームモード
	if (mode == "VERTICAL") {
		transitionData.nextMode = GameMode::VERTICAL;
	}
	else if (mode == "SIDE") {
		transitionData.nextMode = GameMode::SIDE;
	}
	else if (mode == "OMNIDIRECTIONAL") {
		transitionData.nextMode = GameMode::OMNIDIRECTIONAL;
	}
	else {
		return;
	}

	transitionData.transitionTime = time; // 遷移にかかるフレーム

	transitionData.beginCameraQuaternion = camera->get_transform().get_quaternion();
	transitionData.beginOffset = camera->get_offset();
	transitionData.timer = 0;

	playerMoveStateFunc(&transitionData);

	transitionData.isTransitioning = true;

	nowMode = GameMode::TRANSITION;

	Log(std::format("[GameModeManager] Set GameMode : {}\n", static_cast<int>(nowMode)));

}

void GameModeManager::set_player_func(std::function<void(TransitionData*)> func) {
	playerMoveStateFunc = func;
}

void GameModeManager::set_camera(GazerCamera* camera_) {
	camera = camera_;
}

void GameModeManager::set_next_angle(Vector3&& angle) {
	transitionData.afterCameraQuaternion = Quaternion::EulerDegree(angle);
}

void GameModeManager::set_next_offset(Vector3&& offset) {
	transitionData.afterOffset = offset;
}

GameMode GameModeManager::get_mode() const {
	return nowMode;
}

const TransitionData& GameModeManager::get_transition_data() const {
	return transitionData;
}

#ifdef _DEBUG

#include <imgui.h>
#include <ranges>

void GameModeManager::debug_gui() {
	static const std::vector<std::pair<std::string, GameMode>> gameModeList{
		{ "NANE", GameMode::NANE },
		{ "VERTICAL",GameMode::VERTICAL },
		{ "SIDE",GameMode::SIDE },
		{ "OMNIDIRECTIONAL",GameMode::OMNIDIRECTIONAL},
		{ "TRANSITION",GameMode::TRANSITION },
		{ "DEBUG_",GameMode::DEBUG_ },
		{ "EDITOR_",GameMode::EDITOR_ },
	};

	ImGui::Begin("GameMode");
	for (const auto & list : gameModeList) {
		if (ImGui::RadioButton(list.first.c_str(), list.second == nowMode)) {
			set_next_mode(list.first, 0);
		}
	}
	ImGui::End();
}
#endif // _DEBUG
