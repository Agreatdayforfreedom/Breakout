#ifndef POWER_UP_H
#define POWER_UP_H

#include <cglm/cglm.h>
#include "texture.h"
#include "game_object.h"

typedef struct 
{
    GameObject inherit;
    char* Type;
    float       Duration;
    bool        Activated;
} PowerUp;
void new_PowerUp(PowerUp* self, char* type, vec3 color, float duration, vec2 position, Texture2D texture);

#endif

