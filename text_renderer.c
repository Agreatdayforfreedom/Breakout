

//#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "text_renderer.h"
#include "resource_manager.h"


void init_TextRenderer(TextRenderer* self, ResourceManager* RM, unsigned int width, unsigned int height)
{
    // load and configure shader
    self->TextShader = LoadShader(RM, 3);
    mat4 projection;

    glm_ortho(0.0f, width, height, 0.0f, -1.0f,1.0f, projection);
    use(&self->TextShader);
    setMatrix4(&self->TextShader, "projection", projection, false);
    setInteger(&self->TextShader, "text", 0, false);
    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &self->VAO);
    glGenBuffers(1, &self->VBO);
    glBindVertexArray(self->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LoadText(TextRenderer* self, char* font, unsigned int fontSize)
{
    // first clear the previously loaded Characters
    memset(self->Characters, 0, sizeof self->Characters);
    // then initialize and load the FreeType library
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) // all functions return a value different than 0 whenever an error occurred
        printf("ERROR::FREETYPE: Could not init FreeType Library");
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font, 0, &face))
        printf("ERROR::FREETYPE: Failed to load font");
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // then for the first 128 ASCII characters, pre-load/compile their characters and store them
    for (GLubyte c = 0; c < 128; c++)  
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            printf("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        

        Character character = {
            .TextureID=texture,
            .Size= {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            .Bearing={face->glyph->bitmap_left, face->glyph->bitmap_top},
            .Advance=face->glyph->advance.x
        };
        printf("%i, %c\n", c,c);
        self->Characters[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void RenderText(TextRenderer* self, char* text, float x, float y, float scale, vec3 color)
{
    // activate corresponding render state	
    use(&self->TextShader);
    setVector3f(&self->TextShader,"textColor", color,false);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(self->VAO);

    // iterate through all characters
    unsigned int c;
    for (c = 0; c < strlen(text); c++)
    {
        char fromChar = text[c];
        Character ch = self->Characters[fromChar];
        float xpos = x + ch.Bearing[0] * scale;
        float ypos = y + (self->Characters['H'].Bearing[1] - ch.Bearing[1]) * scale;

        float w = ch.Size[0] * scale;
        float h = ch.Size[1] * scale;

        //// update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}