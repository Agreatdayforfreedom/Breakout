#pragma once
#pragma warning(disable: 4996)


/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "texture.h"
#include "sprite_renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
typedef struct {
    // object state
    vec2        Position, Size, Velocity;
    vec3        Color;
    float       Rotation;
    bool        IsSolid;
    bool        Destroyed;
    // render state
    Texture2D   Sprite;
    // constructor(s)
} GameObject;
//GameObject();
void Game_Object(GameObject *self, vec2 pos, vec2 size, vec2 velocity, vec3 color, Texture2D sprite);
// draw sprite
void Draw_GameObject(GameObject*self, SpriteRenderer *renderer);

#endif

