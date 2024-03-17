#pragma once
#pragma warning(disable: 4996)

#ifndef SHADER_H
#define SHADER_H
#include <stdio.h>

#include <stdbool.h>
#include <glad/glad.h>
#include <cglm/cglm.h>


// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.

typedef struct {
    unsigned int ID;
} Shader;

void    use(Shader* self);
void    compile(Shader* self, const char* vertexSource, const char* fragmentSource, const char* geometrySource); // note: geometry source code is optional 
void    checkCompileErrors(Shader* self, unsigned int object, char* type);
void    setFloat(Shader* self, const char* name, float value, bool useShader);
void    setInteger(Shader* self, const char* name, int value, bool useShader);
void    setVector2f(Shader* self, const char* name, const vec2 value, bool useShader);
void    setVector3f(Shader* self, const char* name, const vec3 value, bool useShader);
void    setVector4f(Shader* self, const char* name, const vec4 value, bool useShader);
void    setMatrix4(Shader* self, const char* name, const mat4 matrix, bool useShader);

#endif