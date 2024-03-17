
#include "shader.h"

void use(Shader* self)
{
    glUseProgram(self->ID);
}

void compile(Shader* self, const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    printf("COMPILING SHADER");
    unsigned int sVertex, sFragment, gShader;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(&self, sVertex, "VERTEX");
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(&self, sFragment, "FRAGMENT");
    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != NULL)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(&self, gShader, "GEOMETRY");
    }
    // shader program
    self->ID = glCreateProgram();
    glAttachShader(self->ID, sVertex);
    glAttachShader(self->ID, sFragment);
    if (geometrySource != NULL)
        glAttachShader(self->ID, gShader);
    glLinkProgram(self->ID);
    checkCompileErrors(&self, self->ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != NULL)
        glDeleteShader(gShader);
}

void setFloat(Shader* self, const char* name, float value, bool useShader)
{
    if (useShader)
        use(&self);
    glUniform1f(glGetUniformLocation(self->ID, name), value);
}
void setInteger(Shader* self, const char* name, int value, bool useShader)
{
    if (useShader)
        use(&self);
    glUniform1i(glGetUniformLocation(self->ID, name), value);
}

void setVector2f(Shader* self, const char* name, vec2 value, bool useShader)
{
    if (useShader)
        use(&self);
    glUniform2f(glGetUniformLocation(self->ID, name), value[0], value[1]);
}

void setVector3f(Shader* self, char* name, vec3 value, bool useShader)
{
    if (useShader)
        use(&self);
    glUniform3f(glGetUniformLocation(self->ID, name), value[0], value[1], value[2]);
}

void setVector4f(Shader* self, const char* name, vec4 value, bool useShader)
{
    if (useShader)
        use(&self);
    glUniform4f(glGetUniformLocation(self->ID, name), value[0], value[1], value[2], value[3]);
}
void setMatrix4(Shader* self, const char* name, mat4 matrix, bool useShader)
{
    if (useShader)
        use(&self);
    glUniformMatrix4fv(glGetUniformLocation(self->ID, name), 1, false, (float*)matrix);
}


void checkCompileErrors(Shader* self, unsigned int object, char* type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            printf("| ERROR::SHADER: Compile-time error: Type: %i \n %s \n -- --------------------------------------------------- -- ", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            printf("| ERROR::Shader: Link-time error: Type: %i \n %s \n -- --------------------------------------------------- -- \n", type, infoLog);
        }
    }
}