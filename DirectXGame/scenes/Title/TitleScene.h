#pragma once

#include <BaseScene.h>

#include <memory>

class Sprite;
class Input;
class DirectXCommon;

class TitleScene : public BaseScene {
public:

public:
	void initialize() override;

	void begin() override;

	void update() override;

	void begin_rendering() override;

	void after_update() override;

	void draw() const override;

private:
	std::unique_ptr<Sprite> whiteSprite;
	std::unique_ptr<Sprite> titleSprite;
	std::unique_ptr<Sprite> startButton;

	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	float timer;
	bool isPressButtonA;
	bool isPrePressButtonA;
};

