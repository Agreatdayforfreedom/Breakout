#ifndef POWER_UP_H
#define POWER_UP_H
#include <string>

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "game_object.h"


vec2 POWERUP_SIZE = {60.0f, 20.0f};
vec2 VELOCITY = { 0.0f, 150.0f};



typedef struct 
{
    GameObject inherit;
    char* Type;
    float       Duration;
    bool        Activated;
} PowerUp;
void new_PowerUp(PowerUp* self, char* type, vec3 color, float duration, vec2 position, Texture2D texture) {

    Game_Object(&self->inherit, position, POWERUP_SIZE, VELOCITY, color, texture);
    self->Type = type; 
    self->Duration = duration; 
    self->Activated = true;

}

#endif

