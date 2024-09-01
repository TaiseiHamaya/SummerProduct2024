#pragma once

#include <sstream>
#include <functional>

#include <TimedCall.h>

class GameModeManager;
class BaseEnemy;

class GameTimeline {
public:

public:
	void initialize();
	void update();


private:
	void next_command();
	void load(const std::string& fileName);

public:
	void set_spawn_func(std::function<void(const std::string&)> spawnFunction_);
	void set_mode(GameModeManager* gameMode_);
	void set_enemies(const std::list<std::unique_ptr<BaseEnemy>>* enemies);
	bool is_end_timeline() const;

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

#ifdef _DEBUG
	std::string timelineFileName;
	bool isUpdate;
#endif // _DEBUG
};
