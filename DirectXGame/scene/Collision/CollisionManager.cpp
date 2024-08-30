#include "CollisionManager.h"

#include <ranges>

#include "Collision/SphereCollider.h"

void CollisionManager::begin_flame() {
	colliderList.clear();
}

void CollisionManager::update() {
	for (const auto& list : colliderList | std::views::values) {
		list.lock()->update();
	}
}

void CollisionManager::collision(const std::string& groupName1, const std::string& groupName2) {
	auto&& group1Range = colliderList.equal_range(groupName1);

	for (auto&& group1 = group1Range.first; group1 != group1Range.second; ++group1) {
		
		auto&& group2Range = colliderList.equal_range(groupName2);
		for (auto&& group2 = group2Range.first; group2 != group2Range.second; ++group2) {
			test_collision(group1->second.lock(), group2->second.lock());
		}
	}
}

void CollisionManager::register_collider(const std::string& groupName, const std::weak_ptr<BaseCollider>& collider) {
	colliderList.emplace(groupName, collider);
}

bool Collision(SphereCollider* lhs, SphereCollider* rhs) {
	Vector3 distance = lhs->position() - rhs->position();
	float range = lhs->get_radius() + rhs->get_radius();
	return distance.length() <= range;
}

void CollisionManager::test_collision(const std::shared_ptr<BaseCollider>& test1, const std::shared_ptr<BaseCollider>& test2) {
	std::string type1 = test1->type();
	std::string type2 = test2->type();

	bool result = false;

	if (type1 == "Sphere") {
		auto downed1 = static_cast<SphereCollider*>(test1.get());
		if (type2 == "Sphere") {
			auto downed2 = static_cast<SphereCollider*>(test2.get());
			if (Collision(downed1, downed2)) {
				result = true;
			}
		}
	}

	if (result) {
		test1->callback(test2.get());
		test2->callback(test1.get());
	}
}
