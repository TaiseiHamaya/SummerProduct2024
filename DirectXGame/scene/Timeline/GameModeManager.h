#pragma once

#include <sstream>
#include <functional>

#include <Quaternion.h>

enum class GameMode {
	NANE = 0,
	VERTICAL = 1 << 1,
	SIDE = 1 << 2,
	OMNIDIRECTIONAL = 1 << 3,
	TRANSITION = 1 << 4,
#ifdef _DEBUG
	DEBUG_ = 1 << 5,
	EDITOR_ = 1 << 6
#endif // _DEBUG
};

class GazerCamera;

struct TransitionData {
	float transitionTime;
	float timer;
	GameMode nextMode;
	bool isTransitioning;
	Quaternion beginCameraQuaternion;
	Quaternion afterCameraQuaternion;
	Vector3 beginOffset;
	Vector3 afterOffset;

	float parametric() {
		return timer / transitionTime;
	};
};

class GameModeManager {
public:

public:
	void initialize();
	void update();

	void game_mode_command(std::istringstream& command);

private:
	void set_next_mode(const std::string& mode, float time);

public:
	void set_player_func(std::function<void(TransitionData*)> func);
	void set_camera(GazerCamera* camera);
	void set_next_angle(Vector3&& angle);
	void set_next_offset(Vector3&& offset);
	GameMode get_mode() const;
	const TransitionData& get_transition_data() const;

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	GameMode nowMode;

	TransitionData transitionData;

	GazerCamera* camera;
	std::function<void(TransitionData*)> playerMoveStateFunc;
};
