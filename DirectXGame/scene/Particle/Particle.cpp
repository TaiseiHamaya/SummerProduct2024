#include "Particle.h"

#include <random>

#include <GameTimer.h>
#include <Definition.h>

Vector3 RandomOnSphere() {
	static std::random_device seed;
	static std::mt19937 engine{ seed() };
	constexpr std::size_t bits = std::numeric_limits<float>::digits;

	float z = std::generate_canonical<float, bits>(engine) * 2 - 1;

	float r = std::sqrt(1.0f - z * z);

	float theta = std::generate_canonical<float, bits>(engine)* PI2;

	return { r * std::cos(theta),	r * std::sin(theta),z };
}


void ParticleSystem::initialize() {
	emitter = std::make_unique<Emitter>();
	emitter->object = std::make_unique<GameObject>();
	emitter->object->initialize();
}

void ParticleSystem::update() {
	timer += GameTimer::DeltaTime();
	if (timer < emitter->duration) {
		emitter->emitCall.update();
	}
	emitter->object->update();

	for (auto& particle : particles) {
		if (particle) {
			particle->velocity += particle->acceleration * GameTimer::DeltaTime();
			particle->object->get_transform().plus_translate(particle->velocity * GameTimer::DeltaTime());
			particle->lifeCall.update();
		}
	}

	particles.remove_if([](const std::unique_ptr<Particle>& particle) { return particle == nullptr; });
}

void ParticleSystem::begin_rendering() {
	emitter->object->begin_rendering();
	for (auto& particle : particles) {
		particle->object->begin_rendering();
	}
}

void ParticleSystem::draw() const {
	for (auto& particle : particles) {
		particle->object->draw();
	}
}

void ParticleSystem::emit() {
	Vector3 position = emitter->object->get_position();
	for (std::int32_t i = 0; i < emitter->rate; ++i) {
		auto&& newParticle = particles.emplace_back(nullptr);
		newParticle = std::make_unique<Particle>();
		newParticle->velocity = RandomOnSphere() * 5;
		newParticle->acceleration = acceleration;
		newParticle->object = std::make_unique<GameObject>();
		newParticle->object->initialize();
		newParticle->object->get_transform().set_translate(position);
		newParticle->object->set_model(particleModel);
		newParticle->object->begin_rendering();
		newParticle->lifeCall = { [&newParticle]() { newParticle.reset(); }, lifeTime };
	}

	if (emitter->isLoop) {
		emitter->emitCall.restart(emitter->particlesParTime);
	}
}

bool ParticleSystem::is_end() const {
	return timer > emitter->duration && particles.empty();
}

void ParticleSystem::set_emitter(const Vector3& position, float duration, std::int32_t rate, float particlesParTime, bool isLoop) {
	emitter->object->get_transform().set_translate(position);
	emitter->object->begin_rendering();
	emitter->duration = duration;
	emitter->rate = rate;
	emitter->emitCall = { std::bind(&ParticleSystem::emit, this), particlesParTime };
	emitter->particlesParTime = particlesParTime;
	emitter->isLoop = isLoop;
}

void ParticleSystem::set_default_particle(float lifeTime_, std::shared_ptr<Model>& model, const Vector3& acceleration_) {
	lifeTime = lifeTime_;
	particleModel = model;
	acceleration = acceleration_;
}
