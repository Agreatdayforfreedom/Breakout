#include "ball.h"
#include "game_object.h"

void BallObject(Ball*self, vec2 pos,
    float radius,
    vec2 velocity,
    Texture2D sprite) {

    Game_Object(&self->inherit, pos, (vec2){ radius * 2.0f, radius * 2.0f }, velocity, (vec3){1.0f,1.0f,1.0f}, sprite);

    self->Radius = radius;
    self->Stuck = true;
    self->Sticky = false;
    self->PassThrough = false;
}

void Move(Ball *self, float dt, unsigned int window_width)
{
    // if not stuck to player board
    if (!self->Stuck)
    {
        // move the ball
        self->inherit.Position[0] += self->inherit.Velocity[0] * dt;
        self->inherit.Position[1] += self->inherit.Velocity[1] * dt;

        if (self->inherit.Position[0] <= 0.0f)
        {
            self->inherit.Velocity[0] = -self->inherit.Velocity[0];
            self->inherit.Position[0] = 0.0f;
        }
        else if (self->inherit.Position[0] + self->inherit.Size[0] >= window_width)
        {
            self->inherit.Velocity[0] = -self->inherit.Velocity[0];
            self->inherit.Position[0] = window_width - self->inherit.Size[0];
        }
        if (self->inherit.Position[1] <= 0.0f)
        {
            self->inherit.Velocity[1] = -self->inherit.Velocity[1];
            self->inherit.Position[1] = 0.0f;
        }

    }
    return self->inherit.Position;
}

void Reset(Ball* self,vec2 position, vec2 velocity)
{
    self->inherit.Position[0] = position[0];
    self->inherit.Position[1] = position[1];
    self->inherit.Velocity[0] = velocity[0];
    self->inherit.Velocity[1] = velocity[1];
    self->Stuck = true;
}