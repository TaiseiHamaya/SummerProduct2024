#include "GameModeManager.h"

#include <string>

#include <Utility.h>

#include <GameTimer.h>
#include <Camera3D.h>

void GameModeManager::initialize() {
#ifdef _DEBUG
	gameMode = GameMode::DEBUG_;
#else
	gameMode = GameMode::NANE;
#endif // _DEBUG
}

void GameModeManager::update() {
	if (gameMode == GameMode::TRANSITION) {
		transitionData.timer += GameTimer::DeltaTime();
		if (transitionData.transitionTime <= transitionData.timer) {
			camera->get_transform().set_rotate(transitionData.afterCameraQuaternion);
			playerMoveStateFunc(&transitionData);
			gameMode = transitionData.nextMode;
			Log(std::format("[GameModeManager] Set GameMode : {}\n", static_cast<int>(gameMode)));
			return;
		}
		camera->get_transform().set_rotate(
			Quaternion::Slerp(
				transitionData.beginCameraQuaternion,
				transitionData.afterCameraQuaternion,
				transitionData.parametric()
			)
		);
	}
#ifdef _DEBUG
	else if (gameMode == GameMode::DEBUG_) {

	}
	else if (gameMode == GameMode::EDITOR_) {

	}
#endif // _DEBUG
}

void GameModeManager::game_mode_command(std::istringstream& command) {
	std::string word;
	std::getline(command, word, ',');
	GameMode nextMode = GameMode::NANE;
	// 次のゲームモード
	if (word == "VERTICAL") {
		nextMode = GameMode::VERTICAL; 
	}
	else if (word == "SIDE") {
		nextMode = GameMode::SIDE;
	}
	else if (word == "OMNIDIRECTIONAL") {
		nextMode = GameMode::OMNIDIRECTIONAL;
	}
	else {
		assert(false);
	}
	std::getline(command, word, ',');
	transitionData.transitionTime = std::stof(word); // 遷移にかかるフレーム

	transitionData.beginCameraQuaternion = camera->get_transform().get_quaternion();
	transitionData.timer = 0;

	transitionData.nextMode = GameMode::TRANSITION;

	playerMoveStateFunc(&transitionData);

	transitionData.nextMode = nextMode;

	gameMode = GameMode::TRANSITION;

	Log(std::format("[GameModeManager] Set GameMode : {}\n", static_cast<int>(gameMode)));
}

void GameModeManager::set_player_func(std::function<void(TransitionData*)> func) {
	playerMoveStateFunc = func;
}

void GameModeManager::set_camera(Camera3D* camera_) {
	camera = camera_;
}

void GameModeManager::set_next_angle(Vector3&& angle) {
	transitionData.afterCameraQuaternion = Quaternion::EulerDegree(angle);
}

#ifdef _DEBUG

#include <imgui.h>

void GameModeManager::debug_gui() {
	ImGui::Begin("GameMode");
	ImGui::Text("GameMode : %d", static_cast<int>(gameMode));
	ImGui::End();
}
#endif // _DEBUG
