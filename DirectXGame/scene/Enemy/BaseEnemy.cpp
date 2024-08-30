#include "BaseEnemy.h"

#include <fstream>

#include <Utility.h>
#include <Definition.h>

#include "Timeline/GameModeManager.h"
#include "Collision/SphereCollider.h"
#include "Enemy/EnemyMoveState/EnemyMoveStop.h"
#include "Enemy/EnemyMoveState/EnemyMoveLinear.h"
#include "Enemy/EnemyMoveState/EnemyMoveCircular.h"

void BaseEnemy::initialize() {
	hierarchy.Initialize();

	attackCall = {
		std::bind(&BaseEnemy::attack, this), 0
	};
	moveCall = {
		std::bind(&BaseEnemy::next_command, this), 0
	};

	isAttack = false;
	isDead = false;

	moveState = std::make_unique<EnemyMoveStop>();

	auto tempCollider = std::make_shared<SphereCollider>();
	tempCollider->set_matrix(hierarchy.matWorld_);
	tempCollider->set_callback(std::bind(&BaseEnemy::on_collision, this, std::placeholders::_1));
	tempCollider->set_radius(1.0f);
	collider = std::move(tempCollider);

	hitpoint = 5;
}

void BaseEnemy::update() {
	if (hitpoint <= 0) {
		isDead = true;
	}

	moveCall.update();
	moveState->update();
	if (isAttack) {
		attackCall.update();
	}
}

void BaseEnemy::load_move(const std::string& fileName) {
	std::ifstream file{};
	file.open("./Resources/timeline/Enemy/MovementsFile/" + fileName);
	if (!file.is_open()) {
		Log(std::format("[Enemy] Movements file \'{}\' is not found.", fileName));
		return;
	}

	moveCommand << file.rdbuf();

	file.close();
}

void BaseEnemy::next_command() {
	std::string line;
	while (std::getline(moveCommand, line)) {
		std::istringstream lineStream{ line };
		std::string word;

		std::getline(lineStream, word, ',');

		if (word.empty() || word.starts_with("//")) {
			continue;
		}
		else if (word == "MOVE") {
			create_move(lineStream);
		}
		else if (word == "SHOT") {
			std::getline(lineStream, word, ',');
			if (word == "YES") {
				isAttack = true;
			}
			else if (word == "NO") {
				isAttack = false;
			}
		}
		else if (word == "WAIT") {
			std::getline(lineStream, word, ',');
			moveCall.restart(std::stof(word));
			break;
		}
		else if (word == "DESTROY") {
			isDead = true;
		}
	}
}

void BaseEnemy::create_move(std::istringstream& command) {
	std::string word;

	std::getline(command, word, ',');

	auto nowGameMode = modeManager->get_mode();
	if (nowGameMode == GameMode::TRANSITION) {
		nowGameMode = modeManager->get_transition_data().nextMode;
	}

	if (word == "STOP") {
		auto&& newState = std::make_unique<EnemyMoveStop>();
		if (nowGameMode == GameMode::VERTICAL || nowGameMode == GameMode::OMNIDIRECTIONAL) {
			newState->set_stop_mode(StopMode::LOOKAROUND);
		}
		else if (nowGameMode == GameMode::SIDE) {
			newState->set_stop_mode(StopMode::UPDOWN);
		}
		moveState = std::move(newState);
	}
	else if (word == "LINEAR") {
		auto&& newState = std::make_unique<EnemyMoveLinear>();
		Vector2 base;
		std::getline(command, word, ',');
		base.x = std::stof(word);
		std::getline(command, word, ',');
		base.y = std::stof(word);
		std::getline(command, word, ',');
		float speed = std::stof(word);

		Vector3 moveDirection;
		if (nowGameMode == GameMode::VERTICAL || nowGameMode == GameMode::OMNIDIRECTIONAL) {
			moveDirection = { base.x, 0, base.y };
		}
		else if (nowGameMode == GameMode::SIDE) {
			moveDirection = { 0, base.y, base.x };
		}

		newState->initialize(moveDirection.normalize_safe(1e-4f, CVector3::ZERO), speed);
		moveState = std::move(newState);
	}
	else if (word == "CIRCULAR") {
		auto&& newState = std::make_unique<EnemyMoveCircular>();
		Vector2 base;
		std::getline(command, word, ',');
		base.x = std::stof(word);
		std::getline(command, word, ',');
		base.y = std::stof(word);
		std::getline(command, word, ',');
		float degree = std::stof(word);

		Vector3 moveDirection;
		Vector3 axis;
		if (nowGameMode == GameMode::VERTICAL || nowGameMode == GameMode::OMNIDIRECTIONAL) {
			moveDirection = { base.x, 0, base.y };
			axis = CVector3::BASIS_Y;
		}
		else if (nowGameMode == GameMode::SIDE) {
			moveDirection = { 0, base.y, base.x };
			axis = CVector3::BASIS_X;
		}

		newState->initialize(moveDirection, axis, degree * ToRadian);
		moveState = std::move(newState);
	}
	moveState->set_enemy(this);
}

void BaseEnemy::attack() {
	Vector3 forward = Transform3D::HomogeneousVector(CVector3::BASIS_Z, hierarchy.matWorld_);
	attackFunction(get_position(), forward);
	attackCall.restart(1.0f);
}

void BaseEnemy::set_attack_func(const std::function<void(const Vector3&, const Vector3&)>& func) {
	attackFunction = func;
}

void BaseEnemy::set_game_mode_manager(const GameModeManager* manager) {
	modeManager = manager;
}

void BaseEnemy::on_collision([[maybe_unused]] const BaseCollider* collider_) {
	--hitpoint;
}

std::unique_ptr<BaseEnemy> BaseEnemy::Create() {
	return std::make_unique<BaseEnemy>();
}
