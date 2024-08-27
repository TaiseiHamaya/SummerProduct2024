#include "EnemyManager.h"

#include <format>
#include <fstream>

#include <Utility.h>
#include <Definition.h>

#include <Enemy/BaseEnemy.h>
#include <Enemy/FastEnemy.h>
#include <Enemy/ToPlayerEnemy.h>
#include <Timeline/GameModeManager.h>

EnemyManager::EnemyManager() = default;

EnemyManager::~EnemyManager() = default;

void EnemyManager::initialize() {
	create_enemy_data();
}

void EnemyManager::update() {
	for (auto&& itr = enemies.begin(); itr != enemies.end(); ++itr) {
		itr->get()->update();
	}
	enemies.remove_if([](const std::unique_ptr<BaseEnemy>& enemy) { return enemy->is_dead(); });
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

	std::stringstream fileStream;
	fileStream << file.rdbuf();

	file.close();

	std::string line;
	while (std::getline(fileStream, line, '\n')) {
		std::stringstream lineStream{ line };
		std::string enemyTypeName;
		std::getline(lineStream, enemyTypeName, ',');

		if (enemyTypeName.empty() || enemyTypeName.starts_with("//")) {
			continue;
		}

		std::string enemyFileName;
		std::getline(lineStream, enemyFileName, ',');

		std::string word;
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
		auto nowGameMode =  gameModeManager->get_mode();
		if (nowGameMode == GameMode::VERTICAL || nowGameMode == GameMode::OMNIDIRECTIONAL) {
			position = { position2d.x, 0, position2d.y };
			rotation = Quaternion::AngleAxis(CVector3::BASIS_Y, degree * ToRadian);
		}
		else if(nowGameMode == GameMode::SIDE){
			position = { 0, position2d.y, position2d.x };
			rotation = Quaternion::AngleAxis(CVector3::BASIS_X, degree * ToRadian);
		}
		else {
			// do nothing
		}

		pop_enemy(enemyTypeName, enemyFileName, position, rotation);
	}
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

void EnemyManager::create_enemy_data() {
	enemyData = {
		 { "DEFAULT", EnemyData{std::shared_ptr<Model>(Model::Create()),&BaseEnemy::Create}},
		 { "FAST", EnemyData{std::shared_ptr<Model>(Model::Create()),&FastEnemy::Create}},
		 { "PLAYER", EnemyData{std::shared_ptr<Model>(Model::Create()),&ToPlayerEnemy::Create}},
	};
}

void EnemyManager::pop_enemy(const std::string& enemyTypeName, const std::string& fileName, const Vector3& position, const Quaternion& rotation) {
	assert(enemyData.contains(enemyTypeName));
	
	std::unique_ptr<BaseEnemy> newEnemy{ enemyData.at(enemyTypeName).createFunction() };

	newEnemy->initialize();
	newEnemy->set_parent(*field);
	newEnemy->set_model(enemyData[enemyTypeName].model);
	newEnemy->set_attack_func(attackFunction);
	newEnemy->set_game_mode_manager(gameModeManager);
	newEnemy->get_transform().set_translate(position);
	newEnemy->get_transform().set_rotate(rotation);
	newEnemy->load_move(fileName);

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
