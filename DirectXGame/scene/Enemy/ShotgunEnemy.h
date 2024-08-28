#pragma once

#include <Enemy/BaseEnemy.h>

class ShotgunEnemy : public BaseEnemy {
public:
	ShotgunEnemy() = default;
	~ShotgunEnemy() = default;

private:
	void initialize() override;
	void attack() override;

private:
	int shotCount;

public:
	static std::unique_ptr<BaseEnemy> Create();
};

