#pragma once

#include <Enemy/BaseEnemy.h>

class ShotgunEnemy : public BaseEnemy {
public:
	ShotgunEnemy() = default;
	~ShotgunEnemy() = default;

private:
	void initialize() override;
	void attack() override;

	void reset_bullet();

private:
	static constexpr std::uint32_t NUM_BULLETS = 15;
	
	int shotCount;

	float timer;
	struct BulletInfo {
		Quaternion rotation;
		float shotTime;
		bool isShot;
	};
	std::array<BulletInfo, NUM_BULLETS> generateBulletsInfo;

public:
	static std::unique_ptr<BaseEnemy> Create();
};

