#pragma once

#include <Input.h>

#include <optional>

#include <Camera3D.h>

class BaseMoveState {
public:
	BaseMoveState() = default;
	virtual ~BaseMoveState() = default;

	BaseMoveState(const BaseMoveState&) = delete;
	BaseMoveState& operator=(const BaseMoveState&) = delete;
	BaseMoveState(BaseMoveState&&) = delete;
	BaseMoveState& operator=(BaseMoveState&&) = delete;

public:
	virtual void initialize();
	void input();
	virtual Vector3 velocity() const = 0;
	virtual std::optional<Quaternion> quaternion() const = 0;

public:
	void set_camera(const Camera3D* camera_);

protected:
	Input* p_input;
	XINPUT_STATE joyState;
	bool inputResult;
	Vector2 moveStickL;
	Vector2 moveStickR;

	const Camera3D* camera;
};
