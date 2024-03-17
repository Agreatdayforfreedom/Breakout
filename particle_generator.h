#pragma once

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "shader.h"
#include "texture.h"
#include "game_object.h"

#define PARTICLES 500


// Represents a single particle and its state
typedef struct {
    vec2 Position, Velocity;
    vec4 Color;
    float     Life;
} Particle;

Particle New_Particle();

typedef struct {
    Particle particles[PARTICLES];
    unsigned int amount;
    Shader shader;
    Texture2D texture;
    unsigned int VAO;

} ParticleGenerator;

void Generate_Particle(ParticleGenerator* self, Shader shader, Texture2D texture, unsigned int amount);
// update all particles
void Update_Particles(ParticleGenerator *self, float dt, GameObject* object, unsigned int newParticles, vec2 offset);
// render all particles
void Draw_Particles(ParticleGenerator *self );

// initializes buffer and vertex attributes
void init_particles(ParticleGenerator *self);
// returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
unsigned int firstUnusedParticle(ParticleGenerator *self );
// respawns particle
void respawnParticle(ParticleGenerator *self, Particle* particle, GameObject* object, vec2 offset);

#endif

