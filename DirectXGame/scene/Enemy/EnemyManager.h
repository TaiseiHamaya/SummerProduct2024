#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <list>
#include <functional>

#include <Model.h>
#include <Quaternion.h>
#include <TimedCall.h>

class Audio;
class GameObject;
class BaseEnemy;
class GameModeManager;

class EnemyManager {
private:
	struct PopCommand {
		std::stringstream stream;
		TimedCall<void(void)> call;
	};

public: // コンストラクタ
	EnemyManager();
	~EnemyManager();

public: // publicメンバ関数
	void initialize();
	void update();
	void begin_rendering();
	void draw() const;

	void load_pop_file(const std::string& fileName);

public:
	void set_field(const GameObject& rhs);
	void set_game_mode_manager(const GameModeManager* manager);
	void set_attack_function(std::function<void(const Vector3&, const Vector3&)>&& func);
	const std::list<std::unique_ptr<BaseEnemy>>& enemy_list() const;

private:
	void next_pop_command();
	void create_enemy_data();
	void pop_enemy(
		const std::string& enemyTypeName, 
		const std::string& fileName, 
		const Vector3& position, 
		const Quaternion& rotation
	);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private: // メンバ変数
	const GameObject* field;
	const GameModeManager* gameModeManager;

	std::function<void(const Vector3&, const Vector3&)> attackFunction;
	struct EnemyData {
		std::shared_ptr<Model> model;
		std::function<std::unique_ptr<BaseEnemy>(void)> createFunction;
	};
	std::unordered_map<std::string, EnemyData> enemyData;
	std::list<std::unique_ptr<BaseEnemy>> enemies;

	PopCommand* nowCommand;
	std::list<PopCommand> enemyPopCommands;

	bool isEnemyDead;
	std::uint32_t enemyDeadSoundHandle;
};

