#pragma once

#include "Player/MoveState/BaseMoveState.h"

#include <Vector3.h>

class OmnidirectionalMove : public BaseMoveState {
public:
	OmnidirectionalMove() = default;
	~OmnidirectionalMove() = default;

public:
	Vector3 velocity() const override;
	std::optional<Quaternion> quaternion() const override;
};
