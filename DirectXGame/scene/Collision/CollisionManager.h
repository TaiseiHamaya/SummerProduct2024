#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "Collision/BaseCollider.h"

class CollisionManager {
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	CollisionManager(const CollisionManager&) = delete;
	CollisionManager(CollisionManager&&) = delete;

public:
	void begin_flame();
	void update();
	void collision(const std::string& groupName1, const std::string& groupName2);
	void register_collider(const std::string& groupName, const std::weak_ptr<BaseCollider>& collider);

private:
	void test_collision(const std::shared_ptr<BaseCollider>& test1, const std::shared_ptr<BaseCollider>& test2);

private:
	std::unordered_multimap<std::string, std::weak_ptr<BaseCollider>> colliderList;
};
