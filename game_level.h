#pragma once
#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <stdio.h>
#include <stdlib.h>

#include "game_object.h"
#include "resource_manager.h"
//#include "spriteRenderer.h"

typedef struct {
    GameObject Bricks[8*15];
} GameLevel;

// level state
// constructor
void Game_Level();
// loads level from file
void Load(GameLevel *self, ResourceManager* RM, const char* file, unsigned int levelWidth, unsigned int levelHeight);
// render level
//void Draw_GameLevel(SpriteRenderer *renderer);
// check if the level is completed (all non-solid tiles are destroyed)
bool IsCompleted();
// initialize level from tile data
void init(GameLevel *self, ResourceManager* RM, unsigned int *tileData, unsigned int levelWidth, unsigned int levelHeight);
void Draw(GameLevel* self, SpriteRenderer* renderer);
#endif // !