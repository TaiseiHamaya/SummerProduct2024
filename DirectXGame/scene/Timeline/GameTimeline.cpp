#include "GameTimeline.h"

#include <fstream>
#include <format>

#include <Utility.h>

#include <Timeline/GameModeManager.h>

void GameTimeline::load(const std::string& fileName) {
	std::ifstream file{};
	file.open(fileName);
	if (!file.is_open()) {
		Log(std::format("[Timeline] Timeline file \'{}\' is not found.", fileName));
		return;
	}

	timelineCommand << file.rdbuf();

	file.close();

	commandCall = {
		std::bind(&GameTimeline::next_command, this), 0
	};
}

void GameTimeline::update() {
	if (isWaitKillALL) {
		if (enemyList->empty()) {
			next_command();
			isWaitKillALL = false;
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

		if (word == "//") {
			continue;
		}
		else if (word == "POP") {
			spawnFunc(lineStream);
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
	}
}

void GameTimeline::wait_command([[maybe_unused]] std::istringstream& lineStream) {
}

void GameTimeline::set_spawn_func(std::function<void(std::istringstream&)> spawnFunction_) {
	spawnFunc = spawnFunction_;
}

void GameTimeline::set_mode(GameModeManager* gameMode_) {
	gameMode = gameMode_;
}

void GameTimeline::set_enemies(const std::list<Enemy>* enemies) {
	enemyList = enemies;
}

#ifdef _DEBUG

#include <imgui.h>

void GameTimeline::debug_gui() {
	ImGui::Begin("Timeline");
	ImGui::Text(std::format("IsWaitKillAll : {:s}({})", isWaitKillALL, enemyList->size()).c_str());
	ImGui::Separator();
	commandCall.debug_gui();
	ImGui::End();
}
#endif // _DEBUG
