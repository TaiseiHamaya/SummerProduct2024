#pragma once

#include <GameObject.h>

#include <list>
#include <memory>

#include <TimedCall.h>

class ParticleSystem {
public:
	ParticleSystem() = default;
	~ParticleSystem() = default;

public:
	void initialize();
	void update();
	void begin_rendering();
	//void late_update();
	void draw() const;

public:
	void emit();
	bool is_end() const;

public:
	void set_emitter(
		const Vector3& position,
		float duration,
		std::int32_t rate,
		float particlesParTime,
		bool isLoop
	);
	void set_default_particle(
		float lifeTime,
		std::shared_ptr<Model>& model,
		const Vector3& acceleration
		);

private:
	struct Particle {
		std::unique_ptr<GameObject> object;
		TimedCall<void(void)> lifeCall;
		Vector3 velocity;
		Vector3 acceleration;
	};

	struct Emitter {
		std::unique_ptr<GameObject> object;
		TimedCall<void(void)> emitCall;
		float duration;
		std::int32_t rate;
		float particlesParTime;
		bool isLoop;
	};

private:
	float timer;

	std::unique_ptr<Emitter> emitter;

	std::shared_ptr<Model> particleModel;
	float lifeTime;
	Vector3 velocity;
	Vector3 acceleration;

	std::list<std::unique_ptr<Particle>> particles;
};
