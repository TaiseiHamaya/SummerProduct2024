#pragma once

#include <sstream>
#include <functional>

#include <TimedCall.h>

class GameModeManager;
class BaseEnemy;

class GameTimeline {
public:

public:
	void load(const std::string& fileName);
	void update();

	void next_command();

public:
	void set_spawn_func(std::function<void(const std::string&)> spawnFunction_);
	void set_mode(GameModeManager* gameMode_);
	void set_enemies(const std::list<std::unique_ptr<BaseEnemy>>* enemies);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	GameModeManager* gameMode;

	std::stringstream timelineCommand;
	TimedCall<void(void)> commandCall;

	std::function<void(const std::string&)> spawnFunc;

	bool isWaitKillALL;
	const std::list<std::unique_ptr<BaseEnemy>>* enemyList;
};
