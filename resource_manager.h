#pragma warning(disable: 4996)

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <stdio.h>

#include "texture.h"
#include "shader.h"

typedef struct {
    Shader Shaders[20];
    Texture2D Textures[20];

} ResourceManager;

void new_ResourceManager(ResourceManager* self);
Shader    loadShaderFromFile(ResourceManager* self);
Texture2D loadTextureFromFile(const char* file, bool alpha);

Shader    LoadShader(ResourceManager* self, unsigned int name);
Shader    GetShader(ResourceManager* self, const char* name);

Texture2D LoadTexture(ResourceManager* self, const char* file, bool alpha, const char* name);
Texture2D GetTexture(ResourceManager* self, const char* name);
void      Clear(ResourceManager* self);

Shader loadParticleShader();
Shader loadPostProcessingShader();

#endif