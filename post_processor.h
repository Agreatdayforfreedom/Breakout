#pragma once
#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"


typedef struct {

    Shader PostProcessingShader;
    Texture2D Texture;

    unsigned int Width, Height;
    bool Confuse, Chaos, Shake;
    unsigned int MSFBO;
    unsigned int FBO; 
    unsigned int RBO;
    unsigned int VAO;
} PostProcessor;

void Post_Processor(PostProcessor* self ,Shader shader, unsigned int width, unsigned int height);
void BeginRender(PostProcessor* self);
void EndRender(PostProcessor* self);
void RenderPP(PostProcessor* self, float time);
void initRenderDataPP(PostProcessor* self);

#endif

