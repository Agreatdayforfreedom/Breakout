

#include "particle_generator.h"

void Generate_Particle(ParticleGenerator* self, Shader shader, Texture2D texture, unsigned int amount) {

    self->shader = shader;
    self->texture = texture;
    self->amount = amount;
    init_particles(self);
}

void Update_Particles(ParticleGenerator* self, float dt, GameObject* object, unsigned int newParticles, vec2 offset)
{
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = firstUnusedParticle(self);
        respawnParticle(self, &self->particles[unusedParticle], object, offset);
    }
    // update all particles
    for (unsigned int i = 0; i < self->amount; ++i)
    {
        Particle *p = &self->particles[i];
        p->Life -= dt; // reduce life
        if (p->Life > 0.0f)
        {	// particle is alive, thus update
            p->Position[0] -= p->Velocity[0] * dt;
            p->Position[1] -= p->Velocity[1] * dt;
            p->Color[3] -= dt * 2.5f; 
        }
    }
}

// render all particles
void Draw_Particles(ParticleGenerator* self)
{
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    use(&self->shader);
    for (unsigned int i = 0; i<PARTICLES; i++)
    {
        Particle particle = self->particles[i];
        if (particle.Life > 0.0f)
        {
            setVector2f(&self->shader, "offset", particle.Position, false);
            setVector4f(&self->shader, "color", particle.Color, false);
            Bind(&self->texture);
            glBindVertexArray(self->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void init_particles(ParticleGenerator* self)
{
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &self->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(self->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < self->amount; ++i)
        self->particles[i] = New_Particle();
}

Particle New_Particle() {
    Particle particle = {
        .Position = {0.0f, 0.0f}
        ,.Velocity = {0.0f,0.0f}
        ,.Color = {1.0f,1.0f,1.0f,1.0f}
        ,.Life = 0.0f
    };
    return particle;
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int firstUnusedParticle(ParticleGenerator *self)
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < self->amount; ++i) {
        if (self->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (self->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void respawnParticle(ParticleGenerator *self,Particle* particle, GameObject* object, vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    vec2 pos;
    glm_vec2_add(object->Position, offset, pos);
    glm_vec2_adds(pos, random, pos);
    particle->Position[0] = pos[0];
    particle->Position[1] = pos[1];
    glm_vec4_copy((vec4) { rColor, rColor, rColor, 1.0f }, particle->Color);
    particle->Life = 1.0f;
    particle->Velocity[0] = object->Velocity[0] * 0.1f;
    particle->Velocity[0] = object->Velocity[0] * 0.1f;
}

