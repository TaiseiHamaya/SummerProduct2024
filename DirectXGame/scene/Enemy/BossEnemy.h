#pragma once

#include <Enemy/BaseEnemy.h>

class BossEnemy : public BaseEnemy {
public:
	BossEnemy() = default;
	~BossEnemy() = default;

private:
	void initialize() override;
	void update() override;

	void attack() override;

private:


public:
	static std::unique_ptr<BaseEnemy> Create();
};

