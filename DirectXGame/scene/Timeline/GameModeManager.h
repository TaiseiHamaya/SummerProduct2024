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

class Camera3D;

struct TransitionData {
	float transitionTime;
	float timer;
	GameMode nextMode;
	bool isTransitioning;
	Quaternion beginCameraQuaternion;
	Quaternion afterCameraQuaternion;

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

public:
	void set_player_func(std::function<void(TransitionData*)> func);
	void set_camera(Camera3D* camera);
	void set_next_angle(Vector3&& angle);
	GameMode get_mode() const;

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	GameMode nowMode;

	TransitionData transitionData;

	Camera3D* camera;
	std::function<void(TransitionData*)> playerMoveStateFunc;
};
