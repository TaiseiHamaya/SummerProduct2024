#pragma once

#include <sstream>
#include <functional>

class GameModeManager;

class GameTimeline {
public:

public:
	void load(const std::string& fileName);
	void update();

public:
	void set_spawn_func(std::function<void(const std::string&)> spawnFunction_);
	void set_mode(GameModeManager* gameMode_);

private:
	float waitTime;
	std::function<void(const std::string&)> spawnFunc;
	std::stringstream timelineCommand;

	GameModeManager* gameMode;
};
