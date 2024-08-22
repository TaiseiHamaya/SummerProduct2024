#pragma once

#include "Player/MoveState/BaseMoveState.h"

class SideMove : public BaseMoveState {
public:
	SideMove() = default;
	~SideMove() = default;

public:
	Vector3 velocity() const override;
	std::optional<Quaternion> quaternion() const override;

private:
};
