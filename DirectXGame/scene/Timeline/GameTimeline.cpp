#include "GameTimeline.h"

#include <fstream>
#include <format>

#include <GameTimer.h>
#include <Utility.h>

#include <Timeline/GameModeManager.h>

void GameTimeline::load(const std::string& fileName) {
	std::ifstream file{};
	file.open(fileName);
	if (!file.is_open()) {
		Log(std::format("\'Timeline\' {} is not found.", fileName));
		return;
	}

	timelineCommand << file.rdbuf();

	file.close();
}

void GameTimeline::update() {
	waitTime -= GameTimer::DeltaTime();
	if (waitTime > 0) {
		return;
	}
	std::string line;
	while (std::getline(timelineCommand, line)) {
		std::istringstream lineStream{ line };
		std::string word;

		std::getline(lineStream, word, ',');

		if (word == "//") {
			continue;
		}
		else if (word == "POP") {
			std::getline(lineStream, word, ',');
			spawnFunc(word);
		}
		else if (word == "WAIT") {
			std::getline(lineStream, word, ',');
			waitTime = std::stof(word);

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

void GameTimeline::set_spawn_func(std::function<void(const std::string&)> spawnFunction_) {
	spawnFunc = spawnFunction_;
}

void GameTimeline::set_mode(GameModeManager* gameMode_) {
	gameMode = gameMode_;
}
