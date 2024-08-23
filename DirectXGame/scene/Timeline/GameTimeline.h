#pragma once

#include <sstream>
#include <functional>

#include <TimedCall.h>

class GameModeManager;
class Enemy;

class GameTimeline {
public:

public:
	void load(const std::string& fileName);
	void update();

	void next_command();

	void wait_command(std::istringstream& lineStream);

public:
	void set_spawn_func(std::function<void(std::istringstream&)> spawnFunction_);
	void set_mode(GameModeManager* gameMode_);
	void set_enemies(const std::list<Enemy>* enemies);

private:
	GameModeManager* gameMode;

	std::stringstream timelineCommand;
	TimedCall<void(void)> commandCall;

	std::function<void(std::istringstream&)> spawnFunc;

	bool isWaitKillALL;
	const std::list<Enemy>* enemyList;
};
