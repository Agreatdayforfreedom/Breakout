#include "texture.h"


void new_Texture2D(Texture2D* self)
{
    self->Width = 0;
    self->Height = 0;
    self->Internal_Format = GL_RGB;
    self->Image_Format = GL_RGB;
    self->Wrap_S = GL_REPEAT;
    self->Wrap_T = GL_REPEAT;
    self->Filter_Min = GL_LINEAR;
    self->Filter_Max = GL_LINEAR;
    glGenTextures(1, &self->ID);
}

void Generate(Texture2D* self, unsigned int width, unsigned int height, unsigned char* data)
{
    self->Width = width;
    self->Height = height;
    // create Texture

    glBindTexture(GL_TEXTURE_2D, self->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, self->Internal_Format, width, height, 0, self->Image_Format, GL_UNSIGNED_BYTE, data);
   
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, self->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, self->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, self->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, self->Filter_Max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Bind(Texture2D* self) {
    glBindTexture(GL_TEXTURE_2D, self->ID);
}

