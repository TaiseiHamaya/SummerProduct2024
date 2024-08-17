#pragma once

#include "Player/MoveState/BaseMoveState.h"

#include <Camera3D.h>
#include <Vector3.h>

class OmnidirectionalMove : public BaseMoveState {
public:
	OmnidirectionalMove() = default;
	~OmnidirectionalMove() = default;

public:
	//void initialize() override;
	Vector3 velocity() const override;
	std::optional<Quaternion> quaternion() const override;

public:
	void set_camera(const Camera3D* camera_);

private:
	const Camera3D* camera;
};
