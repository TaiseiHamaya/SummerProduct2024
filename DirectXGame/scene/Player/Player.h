#pragma once

#include <GameObject.h>

#include <memory>
#include <functional>

#include <Player/MoveState/BaseMoveState.h>

class Input;
class Camera3D;
struct TransitionData;

class Player : public GameObject {
public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void initialize() override;
	void update() override;

	void attack();

public:
	void default_data(Vector3&& position);
	void set_move_state(TransitionData* transitionData);
	void set_attack_func(const std::function<void(void)>& func);
	void set_camera(Camera3D* camera_);

public:
	void on_collision(const BaseCollider* collider);

public:
#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

private: // メンバ変数
	Input* input = nullptr;

	std::unique_ptr<BaseMoveState> moveState;
	Vector3 velocity;

	float attackTimer;
	std::function<void(void)> attackFunction;

	Camera3D* camera;
};