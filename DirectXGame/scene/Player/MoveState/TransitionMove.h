#pragma once

#include <Transform3D.h>

#include "Player/MoveState/BaseMoveState.h"

struct TransitionData;

class TransitionMove : public BaseMoveState {
public:
	TransitionMove() = default;
	~TransitionMove() = default;

public:
	void input(const XINPUT_STATE& joyState) override;
	Vector3 velocity() const override;
	std::optional<Quaternion> quaternion() const override;

public:
	void set_transition_data(TransitionData* transitionData_);
	void move_now_move_state(std::unique_ptr<BaseMoveState>&& moveState_);
	void set_player_transform(const Transform3D& playerTransform);

private:
	std::unique_ptr<BaseMoveState> beforeMoveState;
	TransitionData* transitionData;
	
	struct PlayerTransitionData {
		Transform3D befor;
	} playerTransitionData;
};
