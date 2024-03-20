#include "power_up.h"

vec2 POWERUP_SIZE = { 60.0f, 20.0f };
vec2 VELOCITY = { 0.0f, 150.0f };

void new_PowerUp(PowerUp* self, char* type, vec3 color, float duration, vec2 position, Texture2D texture) {

     Game_Object(&self->inherit, position, POWERUP_SIZE, VELOCITY, color, texture);
     self->Type = type;
     self->Duration = duration;
     self->Activated = true;

}