#include "TransitionMove.h"

#include "Timeline/GameModeManager.h"

void TransitionMove::set_transition_data(TransitionData* transitionData_) {
	transitionData = transitionData_;
}

void TransitionMove::move_now_move_state(std::unique_ptr<BaseMoveState>&& moveState_) {
	beforeMoveState = std::move(moveState_);
}

void TransitionMove::set_player_transform(const Transform3D& playerTransform) {
	playerTransitionData.befor.copy(playerTransform);
}

void TransitionMove::input(const XINPUT_STATE& joyState) {
	if (beforeMoveState) {
		beforeMoveState->input(joyState);
	}
}

Vector3 TransitionMove::velocity() const {
	// 遷移に必要な情報が足りない場合何もしない
	if (!beforeMoveState || !transitionData) {
		return CVector3::ZERO;
	}


	Vector3 nextVelocity = beforeMoveState->velocity();
	switch (transitionData->nextMode) {
		// 縦, 見下ろしの場合はxz平面に戻す
	case GameMode::VERTICAL:
	case GameMode::OMNIDIRECTIONAL:
		nextVelocity.y = -playerTransitionData.befor.get_translate().y / transitionData->transitionTime;

		nextVelocity.y = transitionData->transitionTime ? -playerTransitionData.befor.get_translate().y / transitionData->transitionTime : 0;
		break;
		// 横の場合はyz平面に戻す
	case GameMode::SIDE:
		nextVelocity.x = transitionData->transitionTime ? -playerTransitionData.befor.get_translate().x / transitionData->transitionTime : 0;
		break;
#ifdef _DEBUG
	case GameMode::DEBUG_:
	case GameMode::EDITOR_:
		// do nothing
		break;
#endif // _DEBUG
	case GameMode::TRANSITION:
	case GameMode::NANE:
	default:
		assert(false);
		break;
	}
	return nextVelocity;
}

std::optional<Quaternion> TransitionMove::quaternion() const {
	// 遷移に必要な情報が足りない場合何もしない
	if (!beforeMoveState || !transitionData) {
		return std::nullopt;
	}

	switch (transitionData->nextMode) {
		// 縦横の場合は正面に戻す
	case GameMode::VERTICAL:
	case GameMode::SIDE:
		if (transitionData->transitionTime <= transitionData->timer) {
			return CQuaternion::IDENTITY;
		}
		return Quaternion::Slerp(
			playerTransitionData.befor.get_quaternion(),
			CQuaternion::IDENTITY,
			transitionData->parametric()
		);
		break;
		// 見下ろしの場合はカメラ正面を向ける
	case GameMode::OMNIDIRECTIONAL:
	{
		Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
		Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
		Quaternion rotation = Quaternion::FromToRotation(xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z), CVector3::BASIS_Z);

		if (transitionData->transitionTime <= transitionData->timer) {
			return rotation;
		}
		auto result = Quaternion::Slerp(
			playerTransitionData.befor.get_quaternion(),
			rotation,
			transitionData->parametric()
		);
		return result;
	}
	break;
#ifdef _DEBUG
	case GameMode::DEBUG_:
	case GameMode::EDITOR_:
		// do nothing
		break;
#endif // _DEBUG
	case GameMode::TRANSITION:
	case GameMode::NANE:
	default:
		assert(false);
		break;
	}
	return std::nullopt;
}
