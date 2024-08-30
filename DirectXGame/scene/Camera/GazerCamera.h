#pragma once

#include <Camera3D.h>
#include <TimedCall.h>

class GazerCamera final : public Camera3D {
public:
	void initialize() override;
	void update() override;

public:
	void set_offset(const Vector3& offset_);
	const Vector3& get_offset() const;

	void do_shake();

#ifdef _DEBUG
	void debug_gui() override;
#endif // _DEBUG

private:
	Vector3 offset;

	Vector3 shake;
	TimedCall<void(void)> shakeCall;
};

