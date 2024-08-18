#pragma once

#include "Player/MoveState/BaseMoveState.h"

class VerticalMove : public BaseMoveState {
public:
	VerticalMove() = default;
	~VerticalMove() = default;

public:
	Vector3 velocity() const override;
	std::optional<Quaternion> quaternion() const override;

private:
};

