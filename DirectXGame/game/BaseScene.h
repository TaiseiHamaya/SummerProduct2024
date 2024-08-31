#pragma once

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene {
public:
	BaseScene() = default;
	virtual ~BaseScene() = default;

private: /// --------------コピー禁止--------------
	BaseScene(const BaseScene&) = delete;
	BaseScene& operator=(const BaseScene&) = delete;

public:
	virtual void initialize() = 0;

	virtual void begin() = 0;
	virtual void update() = 0;
	virtual void begin_rendering() = 0;
	virtual void after_update() = 0;

	virtual void draw() const = 0;
};