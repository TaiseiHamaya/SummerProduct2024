#pragma once

#include <Enemy/BaseEnemy.h>

class GatlingEnemy : public BaseEnemy {
public:
	GatlingEnemy() = default;
	~GatlingEnemy() = default;

private:
	void initialize() override;
	void attack() override;

	void reset_gatling();

private:
	int shotCount;
	TimedCall<void(void)> gatlingCall;

public:
	static std::unique_ptr<BaseEnemy> Create();
};

