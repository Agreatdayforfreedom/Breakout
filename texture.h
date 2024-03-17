#pragma once
#pragma warning(disable: 4996)

#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdio.h>

#include <glad/glad.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.

typedef struct {
    unsigned int ID;
    unsigned int Width, Height;
    unsigned int Internal_Format;
    unsigned int Image_Format;
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;
} Texture2D;

void new_Texture2D(Texture2D* self);
void Generate(Texture2D* self, unsigned int width, unsigned int height, unsigned char* data);
void Bind(Texture2D* self);

#endif
