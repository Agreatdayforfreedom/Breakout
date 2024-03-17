#pragma once


/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "game_object.h"
#include "texture.h"

typedef struct 
{
    GameObject inherit;
    bool    Sticky, PassThrough;
    float   Radius;
    bool    Stuck;
} Ball;
void BallObject(Ball* self, vec2 pos, float radius, vec2 velocity, Texture2D sprite);
void Move(Ball* self, float dt, unsigned int window_width);
void Reset(Ball*self, vec2 position, vec2 velocity);

#endif

