#pragma once

#include <GameObject.h>
#include <Input.h>

#include <optional>

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
	void set_target(const GameObject* target);

protected:
	const GameObject* gameObject;

	Input* p_input;
	XINPUT_STATE joyState;
	bool inputResult;
	Vector3 moveStickL;
	Vector3 moveStickR;
};
