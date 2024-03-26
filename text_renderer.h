#pragma once
#pragma warning(disable: 4996)

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H


#include <glad/glad.h>
#include <cglm/cglm.h>

#include "texture.h"
#include "shader.h"
#include "resource_manager.h"


typedef struct {
    unsigned int TextureID; // ID handle of the glyph texture
    ivec2   Size;      // size of glyph
    ivec2   Bearing;   // offset from baseline to left/top of glyph
    unsigned int Advance;   // horizontal offset to advance to next glyph
} Character;


typedef struct 
{
    Character Characters[128];
    Shader TextShader;
    unsigned int VAO, VBO;
} TextRenderer;

void init_TextRenderer(TextRenderer* self, ResourceManager* RM, unsigned int width, unsigned int height);
void LoadText(TextRenderer* self,  char* font, unsigned int fontSize);
void RenderText(TextRenderer* self, char* text, float x, float y, float scale, vec3 color);


#endif 