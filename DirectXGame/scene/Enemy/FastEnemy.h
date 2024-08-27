#pragma once

#include <Enemy/BaseEnemy.h>

class FastEnemy : public BaseEnemy {
public:
	FastEnemy() = default;
	~FastEnemy() = default;

private:
	void attack() override;

public:
	static std::unique_ptr<BaseEnemy> Create();
};
