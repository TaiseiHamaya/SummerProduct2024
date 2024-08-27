#include "GameTimeline.h"

#include <fstream>
#include <format>

#include <Utility.h>

#include <Timeline/GameModeManager.h>

void GameTimeline::initialize() {
	load("./Resources/timeline/Timeline.csv");
}

void GameTimeline::update() {
#ifdef _DEBUG
	//auto nowGameMode = gameMode->get_mode();
	if (!isUpdate) {
		return;
	}
#endif // _DEBUG

	if (isWaitKillALL) {
		if (enemyList->empty()) {
			isWaitKillALL = false;
			next_command();
		}
		return;
	}
	commandCall.update();
}

void GameTimeline::next_command() {
	std::string line;
	while (std::getline(timelineCommand, line)) {
		std::istringstream lineStream{ line };
		std::string word;

		std::getline(lineStream, word, ',');

		if (word.empty() || word.starts_with("//")) {
			continue;
		}
		else if (word == "POP") {
			std::getline(lineStream, word, ',');
			spawnFunc(word);
		}
		else if (word == "WAIT") {
			std::getline(lineStream, word, ',');

			if (word == "TIME") {
				std::getline(lineStream, word, ',');
				commandCall.restart(std::stof(word));
			}
			else if(word == "KILL_ALL") {
				isWaitKillALL = true;
			}
			break;
		}
		else if (word == "MODE") {
			//std::getline(lineStream, word, '\n');
			gameMode->game_mode_command(lineStream);
		}
		else if (word == "ANGLE") {
			Vector3 degree;
			std::getline(lineStream, word, ',');
			degree.x = std::stof(word);
			std::getline(lineStream, word, ',');
			degree.y = std::stof(word);
			std::getline(lineStream, word, ',');
			degree.z = std::stof(word);

			gameMode->set_next_angle(std::move(degree));
		}
		else if (word == "OFFSET") {
			Vector3 offset;
			std::getline(lineStream, word, ',');
			offset.x = std::stof(word);
			std::getline(lineStream, word, ',');
			offset.y = std::stof(word);
			std::getline(lineStream, word, ',');
			offset.z = std::stof(word);

			gameMode->set_next_offset(std::move(offset));
		}
	}
}

void GameTimeline::load(const std::string& fileName) {
#ifdef _DEBUG
	timelineFileName = fileName;
#endif // _DEBUG

	isWaitKillALL = false;

	std::ifstream file{};
	file.open(fileName);
	if (!file.is_open()) {
		Log(std::format("[Timeline] Timeline file \'{}\' is not found.", fileName));
		return;
	}

	timelineCommand.clear();
	timelineCommand << file.rdbuf();

	file.close();

	commandCall = {
		std::bind(&GameTimeline::next_command, this), 0
	};
}

void GameTimeline::set_spawn_func(std::function<void(const std::string&)> spawnFunction_) {
	spawnFunc = spawnFunction_;
}

void GameTimeline::set_mode(GameModeManager* gameMode_) {
	gameMode = gameMode_;
}

void GameTimeline::set_enemies(const std::list<std::unique_ptr<BaseEnemy>>* enemies) {
	enemyList = enemies;
}

#ifdef _DEBUG

#include <imgui.h>

void GameTimeline::debug_gui() {
	ImGui::Begin("Timeline");
	ImGui::Text(std::format("IsWaitKillAll : {:s}({})", isWaitKillALL, enemyList->size()).c_str());
	ImGui::Separator();
	commandCall.debug_gui();
	ImGui::Separator();
	ImGui::Checkbox("Update", &isUpdate);
	if (ImGui::Button("Reset timeline")) {
		load(timelineFileName);
	}
	ImGui::End();
}
#endif // _DEBUG
