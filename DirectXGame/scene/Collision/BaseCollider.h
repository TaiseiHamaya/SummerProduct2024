#pragma once

#include <functional>
#include <string>

#include <Vector3.h>
#include <Matrix4x4.h>

class BaseCollider {
public:
	BaseCollider() = default;
	~BaseCollider() = default;

	BaseCollider(const BaseCollider&) = delete;
	BaseCollider(BaseCollider&&) = delete;

public:
	void update();

	Vector3 position() const;

	void callback(const BaseCollider* collision);

	virtual constexpr std::string type() const = 0;

public:
	void set_matrix(const Matrix4x4& matrix);
	void set_callback(std::function<void(const BaseCollider*)> function);

private:
	const Matrix4x4* worldMatrix;
	Vector3 worldPosition;

	std::function<void(const BaseCollider*)> callBackFunction;
};
