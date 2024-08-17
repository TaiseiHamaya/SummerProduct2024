#pragma once

#include <GameObject.h>
#include <Input.h>

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
	virtual Vector3 velocity() const = 0;
	void set_target(const GameObject* target);

protected:
	const GameObject* gameObject;
	Input* input;
};
