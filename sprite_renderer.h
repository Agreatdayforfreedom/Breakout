#pragma once
#pragma warning(disable: 4996)


#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "shader.h"
#include "texture.h"

typedef struct {
    Shader       shader;
    unsigned int quadVAO;
} SpriteRenderer;

void RenderSprite(SpriteRenderer* self, Shader shader);
void DrawSprite(SpriteRenderer* self, Texture2D texture, vec2 position,
    vec2 size, float rotate,
    vec3 color);
void initRenderData(SpriteRenderer* self);
