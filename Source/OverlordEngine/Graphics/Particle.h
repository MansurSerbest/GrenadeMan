#pragma once
#include "../Components/ParticleEmitterComponent.h"
struct GameContext;

class Particle
{
public:
	//Constructor, ParticleEmitterSettings (BY REFERENCE!)
	//the settings of the emitter, passing it by reference makes sure
	//that our particle has the latest settings of the emitter
	Particle(const ParticleEmitterSettings& emitterSettings);
	~Particle(void);

	//Update method
	void Update(const GameContext& context);
	//Init, this method will 'reset' our particle
	void Init(XMFLOAT3 initPosition);

	//Method to retrieve the ParticleVertex information of our particle
	ParticleVertex GetVertexInfo() { return m_VertexInfo; }
	//Method to retrieve the state of our particle
	bool IsActive() { return m_IsActive; }

private:

	//ParticleVertex information, used by the emitter's vertexbuffer
	ParticleVertex m_VertexInfo = ParticleVertex();
	//The emitter's settings (by reference)
	const ParticleEmitterSettings& m_EmitterSettings;
	//An active particle is still alive,
	//An inactive particle can be reused (ReInitialized)
	bool m_IsActive = false;

	//Total energy before going to an inactive state (seconds)
	float m_TotalEnergy = 0.0f;
	//Energy passed since the particle became active
	float m_CurrentEnergy = 0.0f;
	//Percentual grow/scale during the particle's lifetime
	// <0 = shrink, >0 = grow
	float m_SizeGrow = 0.0f;
	//Initial size of the particle
	float m_InitSize = 0.0f;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Particle(const Particle& yRef);
	Particle& operator=(const Particle& yRef);
};

