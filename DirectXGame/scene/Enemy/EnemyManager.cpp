#include "EnemyManager.h"

#include <format>
#include <fstream>

#include <Utility.h>
#include <Definition.h>

#include <Enemy/BaseEnemy.h>
#include <Enemy/FastEnemy.h>
#include <Enemy/ToPlayerEnemy.h>
#include <Enemy/GatlingEnemy.h>
#include <Enemy/ShotgunEnemy.h>
#include <Enemy/BossEnemy.h>
#include <Timeline/GameModeManager.h>

EnemyManager::EnemyManager() = default;

EnemyManager::~EnemyManager() = default;

void EnemyManager::initialize() {
	create_enemy_data();
}

void EnemyManager::update() {
	for (auto&& itr = enemyPopCommands.begin(); itr != enemyPopCommands.end(); ++itr) {
		nowCommand = std::to_address(itr);
		itr->call.update();
	}
	enemyPopCommands.remove_if(
		[](const PopCommand& command) { return command.call.is_finished() && command.stream.eof(); }
	);

	for (auto&& itr = enemies.begin(); itr != enemies.end(); ++itr) {
		itr->get()->update();
	}
	enemies.remove_if(
		[](const std::unique_ptr<BaseEnemy>& enemy) { return enemy->is_dead(); }
	);
}

void EnemyManager::begin_rendering() {
	for (auto&& itr = enemies.begin(); itr != enemies.end(); ++itr) {
		itr->get()->begin_rendering();
	}
}

void EnemyManager::draw() const {
	for (auto&& itr = enemies.begin(); itr != enemies.end(); ++itr) {
		itr->get()->draw();
	}
}

void EnemyManager::load_pop_file(const std::string& fileName) {
	std::ifstream file{};
	file.open("./Resources/timeline/Enemy/PopFile/" + fileName);
	if (!file.is_open()) {
		Log(std::format("[EnemyManager] Enemy pop file \'{}\' is not found.", fileName));
		return;
	}

	enemyPopCommands.emplace_back();
	auto&& command = enemyPopCommands.back();

	command.stream << file.rdbuf();

	file.close();

	command.call = {
		std::bind(&EnemyManager::next_pop_command, this), 0
	};
}

void EnemyManager::set_field(const GameObject& rhs) {
	field = &rhs;
}

void EnemyManager::set_game_mode_manager(const GameModeManager* manager) {
	gameModeManager = manager;
}

void EnemyManager::set_attack_function(std::function<void(const Vector3&, const Vector3&)>&& func) {
	attackFunction = func;
}

const std::list<std::unique_ptr<BaseEnemy>>& EnemyManager::enemy_list() const {
	return enemies;
}

void EnemyManager::next_pop_command() {
	std::string line;
	while (std::getline(nowCommand->stream, line, '\n')) {
		std::stringstream lineStream{ line };
		std::string word;
		std::getline(lineStream, word, ',');

		if (word.empty() || word.starts_with("//")) {
			continue;
		}
		else if (word == "WAIT") {
			std::getline(lineStream, word, ',');
			nowCommand->call.restart(std::stof(word));
			break;
		}
		else if (word == "POP") {
			std::string enemyTypeName;
			std::getline(lineStream, enemyTypeName, ',');
			std::string enemyFileName;
			std::getline(lineStream, enemyFileName, ',');

			Vector2 position2d;
			std::getline(lineStream, word, ',');
			position2d.x = std::stof(word);
			std::getline(lineStream, word, ',');
			position2d.y = std::stof(word);
			float degree;
			std::getline(lineStream, word, ',');
			degree = std::stof(word);

			Vector3 position;
			Quaternion rotation;
			auto nowGameMode = gameModeManager->get_mode();
			if (nowGameMode == GameMode::TRANSITION) {
				nowGameMode = gameModeManager->get_transition_data().nextMode;
			}
			if (nowGameMode == GameMode::VERTICAL || nowGameMode == GameMode::OMNIDIRECTIONAL) {
				position = { position2d.x, 0, position2d.y };
				rotation = Quaternion::AngleAxis(CVector3::BASIS_Y, degree * ToRadian);
			}
			else if (nowGameMode == GameMode::SIDE) {
				position = { 0, position2d.y, position2d.x };
				rotation = Quaternion::AngleAxis(CVector3::BASIS_X, degree * ToRadian);
			}
			else {
				// do nothing
			}

			pop_enemy(enemyTypeName, enemyFileName, position, rotation);
		}
	}
}

void EnemyManager::create_enemy_data() {
	enemyData = {
		 { "DEFAULT", EnemyData{ std::shared_ptr<Model>(Model::CreateFromOBJ("enemies/default", true)), &BaseEnemy::Create }},
		 { "FAST", EnemyData{ std::shared_ptr<Model>(Model::CreateFromOBJ("enemies/default", true)), &FastEnemy::Create }},
		 { "PLAYER", EnemyData{ std::shared_ptr<Model>(Model::CreateFromOBJ("enemies/default", true)), &ToPlayerEnemy::Create }},
		 { "GATLING", EnemyData{ std::shared_ptr<Model>(Model::CreateFromOBJ("enemies/default", true)), &GatlingEnemy::Create }},
		 { "SHOTGUN", EnemyData{ std::shared_ptr<Model>(Model::CreateFromOBJ("enemies/shotgun", true)), &ShotgunEnemy::Create }},
		 { "BOSS", EnemyData{ std::shared_ptr<Model>(Model::CreateFromOBJ("enemies/boss", true)), &BossEnemy::Create}},
	};
}

void EnemyManager::pop_enemy(const std::string& enemyTypeName, const std::string& fileName, const Vector3& position, const Quaternion& rotation) {
	assert(enemyData.contains(enemyTypeName));

	std::unique_ptr<BaseEnemy> newEnemy{ enemyData.at(enemyTypeName).createFunction() };

	newEnemy->set_parent(*field);
	newEnemy->set_model(enemyData[enemyTypeName].model);
	newEnemy->set_attack_func(attackFunction);
	newEnemy->set_game_mode_manager(gameModeManager);
	newEnemy->get_transform().set_translate(position);
	newEnemy->get_transform().set_rotate(rotation);
	newEnemy->load_move(fileName);
	newEnemy->initialize();
	newEnemy->begin_rendering();

	enemies.push_back(std::move(newEnemy));
}

#ifdef _DEBUG

void EnemyManager::debug_gui() {
	ImGui::Begin("EnemyManager");
	ImGui::Text("Enemy : %d", enemies.size());
	if (ImGui::Button("Destroy all enemy")) {
		enemies.clear();
	}
	ImGui::End();
}

#endif // _DEBUG
