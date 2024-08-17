#pragma once

#include "Player/MoveState/BaseMoveState.h"

#include <Vector3.h>

#include <RailField/RailField.h>

class VerticalMove : public BaseMoveState {
public:
	VerticalMove() = default;
	~VerticalMove() = default;

public:
	void initialize() override;
	Vector3 velocity() const override;

public:
	void set_field(const RailField* const field);

private:
	const RailField* field;
};

