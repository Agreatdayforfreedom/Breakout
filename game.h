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
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"
#include "power_up.h"

// Represents the current state of the game
typedef enum {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
} GameState;

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
}Direction;

typedef struct {
    GameState state;
    int keys[1024];
    unsigned int width, height;
    GameLevel levels[4];
    unsigned int level;
    PowerUp powerUps[6];
} Game;

typedef struct {
    bool collide;
    Direction direction;
    vec2 dv;
} Collision;
Collision CheckCollision(Ball* one, GameObject* two);
Direction VectorDirection(vec2 target);
void ActivatePowerUp(PowerUp* powerUp);
void UpdatePowerUps(Game* self, float dt);
bool IsOtherPowerUpActive(PowerUp powerUps[], char* type);


void ConfigGame(Game* self, unsigned int width, unsigned int height);
void Init(Game* self);
void DoCollisions(Game* self);
void ProcessInput(Game* self, float dt);
void Update(Game* self, float dt);
void Render(Game* self);
void SpawnPowerUps(GameObject* block);
void UpdatePowerUps(float dt);

#endif