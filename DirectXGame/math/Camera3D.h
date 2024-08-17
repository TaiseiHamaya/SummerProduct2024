#pragma once

#include <GameObject.h>

#include <Input.h>
#include <DebugCamera.h>

#include <memory>

class Camera3D : public GameObject {
public:
	Camera3D() = default;
	virtual ~Camera3D() = default;

	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

public:
	void initialize() override;

	void begin_rendering() override;

	void set_transform(const Transform3D& transform) noexcept;

	void set_perspective_fov_info(float fovY, float aspectRatio, float nearClip, float farClip) noexcept;

	const ViewProjection& get_view_projection() const;

#ifdef _DEBUG
	virtual void debug_gui();
	void debug_camera();
#endif // _DEBUG

private:
	void make_view_matrix();
	void make_perspectivefov_matrix();

protected:
	std::unique_ptr<ViewProjection> viewProjection;

private:
	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;

#ifdef _DEBUG
	Input* input;
	bool isDebugCameraActive{ false };
	std::unique_ptr<DebugCamera> debugCamera;
#endif // _DEBUG
};