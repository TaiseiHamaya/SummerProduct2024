#pragma once

#include <GameObject.h>

#include <memory>
#include <functional>

#include <TimedCall.h>

#include "Player/MoveState/BaseMoveState.h"

class Sprite;
class Input;
class GazerCamera;
struct TransitionData;

class Player : public GameObject {
public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void initialize() override;
	void update() override;

	void draw_ui() const;

	void attack();

public:
	void default_data(Vector3&& position);
	void set_move_state(TransitionData* transitionData);
	void set_attack_func(const std::function<void(void)>& func);
	void set_camera(GazerCamera* camera_);

public:
	void on_collision(const BaseCollider* collider);

public:
#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

private: // メンバ変数
	Input* input = nullptr;

	static constexpr int MAX_HEALTH = 5;

	int health = MAX_HEALTH;
	struct HealthData {
		std::unique_ptr<Sprite> sprite;
		int flashCount;
		bool isFlashing;
		bool isDraw;
		TimedCall<void(void)> flashingCall;
	};
	std::array<HealthData, MAX_HEALTH> healthData;

	float invincibleTimer;

	std::unique_ptr<BaseMoveState> moveState;
	Vector3 velocity;

	float attackTimer;
	std::function<void(void)> attackFunction;

	std::function<void(void)> cameraShakeFunction;

	Camera3D* camera;
};