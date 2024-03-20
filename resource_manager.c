#include "resource_manager.h"
#include "C:\Users\Poe\Downloads\stb_image.h"
#include <stdlib.h>


Shader LoadShader(ResourceManager* self, unsigned int name)
{
    Shader shader = loadShaderFromFile(&self);
    if (name == 0) {
        printf("COMPILING NORMAL SHADER\n");
        shader = loadShaderFromFile(&self);
    }
    else if (name == 1) {
        printf("COMPILING PARTICLE SHADER\n");
        shader = loadParticleShader();
    }
    else if (name == 2) {
        printf("COMPILING POSTPROCESSING SHADER\n");
        shader = loadPostProcessingShader();
    }



    self->Shaders[name] = shader;
    return self->Shaders[name];
}

Shader GetShader(ResourceManager* self, unsigned int name)
{

    return self->Shaders[name];
}

Texture2D LoadTexture(ResourceManager* self, const char* file, bool alpha, unsigned int name)
{
    self->Textures[name] = loadTextureFromFile(file, alpha);
    return self->Textures[name];
}

Texture2D GetTexture(ResourceManager* self, unsigned int name)
{
    return self->Textures[name];
}

void Clear(ResourceManager* self, unsigned int size)
{
    // (properly) delete all shaders	
    //for (unsigned int i = 0; i < size; i++) {
    //    if (&self->Shaders[i].ID != NULL) {
    //        glDeleteProgram(self->Shaders[i].ID);
    //    }
    //}
    //// (properly) delete all textures
    //for (unsigned int i = 0; i < size; i++) {
    //    if (&self->Textures[i].ID != NULL) {

    //        glDeleteProgram(self->Textures[i].ID);
    //    }
    //}

}
Shader loadShaderFromFile(ResourceManager* self)
{

    size_t length;

    FILE* vfptr; //vertex
    FILE* ffptr; //fregment

    char* vertexCode = "#version 330 core\n"
        "layout(location = 0) in vec4 vertex;\n"
        //"out vec4 multby;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "TexCoords = vec2(vertex.z, vertex.w);\n"
        "gl_Position = projection * model * vec4(vertex.x,vertex.y, 0.0, 1.0);\n"
        "}\0";
    //"TexCoords = vertex.zw;\n"
    char* fragmentCode = "#version 330 core\n"
        "in vec2 TexCoords;\n"
        //"in vec4 multby;\n"
        "out vec4 color;\n"
        "uniform sampler2D imag;\n"
        "uniform vec3 spriteColor;\n"
        "void main()\n"
        "{\n"
        "color =  vec4(spriteColor, 1.0)* texture(imag, TexCoords);\n"
        "}\n\0";

   
    
    Shader shader;

    compile(&shader, vertexCode, fragmentCode, NULL);


    return shader;

}

Shader loadParticleShader() {
    char* vertexCode = "#version 330 core\n"
        "layout(location = 0) in vec4 vertex;\n"
        "out vec2 TexCoords;\n"
        "out vec4 ParticleColor;\n"
        "uniform mat4 projection;\n"
        "uniform vec2 offset;\n"
        "uniform vec4 color;\n"
        "void main()\n"
        "{\n"
            "float scale = 10.0f;\n"
            "TexCoords = vertex.zw;\n"
            "ParticleColor = color;\n"
            "gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);\n"
        "}\n";

    char* fragmentCode = "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "in vec4 ParticleColor;\n"
        "out vec4 color;\n"
        "uniform sampler2D sprite;\n"
        "void main()\n"
        "{\n"
            "color = (texture(sprite, TexCoords) * ParticleColor);\n"
        "}\n";


    Shader shader;

    compile(&shader, vertexCode, fragmentCode, NULL);


    return shader;
}

Shader loadPostProcessingShader() {
    
    char* vertexCode = "#version 330 core\n"
        "layout(location = 0) in vec4 vertex;\n"

    "out vec2 TexCoords;\n"

    "uniform bool  chaos;\n"
    "uniform bool  confuse;\n"
    "uniform bool  shake;\n"
    "uniform float time;\n"

    "void main()\n"
    "{\n"
        "gl_Position = vec4(vertex.xy, 0.0f, 1.0f);\n"
        "vec2 texture = vertex.zw;\n"
        "if (chaos)\n"
        "{\n"
            "float strength = 0.3;\n"
            "vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);\n"
            "TexCoords = pos; \n"
        "}\n"
        "else if (confuse)\n"
        "{\n"
            "TexCoords = vec2(1.0 - texture.x, 1.0 - texture.y);\n"
        "}\n"
        "else\n"
        "{\n"
            "TexCoords = texture;\n"
        "}\n"
        "if (shake)\n"
        "{\n"
           "float strength = 0.01;\n"
           "gl_Position.x += cos(time * 10) * strength;\n"
            "gl_Position.y += cos(time * 15) * strength;\n"
        "}\n"
    "}\n";


    char* fragmentCode = "#version 330 core\n"
        "in  vec2  TexCoords;\n"
    "out vec4  color;\n"

    "uniform sampler2D scene;\n"
    "uniform vec2      offsets[9];\n"
    "uniform int       edge_kernel[9];\n"
    "uniform float     blur_kernel[9];\n"

    "uniform bool chaos;\n"
    "uniform bool confuse;\n"
    "uniform bool shake;\n"

    "void main()\n"
    "{\n"
        "color = vec4(0.0f);\n"
        "vec3 sample[9];\n"
        // sample from texture offsets if using convolution matrix
        "if (chaos || shake)\n"
            "for (int i = 0; i < 9; i++)\n"
               "sample[i] = vec3(texture(scene, TexCoords.st + offsets[i]));\n"

        // process effects
        "if (chaos)\n"
        "{\n"
            "for (int i = 0; i < 9; i++)\n"
                "color += vec4(sample[i] * edge_kernel[i], 0.0f);\n"
            "color.a = 1.0f;\n"
        "}\n"
        "else if (confuse)\n"
        "{\n"
            "color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);\n"
        "}\n"
        "else if (shake)\n"
        "{\n"
            "for (int i = 0; i < 9; i++)\n"
                "color += vec4(sample[i] * blur_kernel[i], 0.0f);\n"
            "color.a = 1.0f;\n"
        "}\n"
        "else\n"
        "{\n"
            "color = texture(scene, TexCoords);\n"
        "}\n"
    "}\n";

    Shader shader;

    compile(&shader, vertexCode, fragmentCode, NULL);

    return shader;
}

Texture2D loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    new_Texture2D(&texture);
    if (alpha != false)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    if(data == NULL){
        printf("\n");
        const char* failureReason = stbi_failure_reason();
        printf("Reason: %s \n", failureReason);
    }
    Generate(&texture, width, height, data);

    // and finally free image data
    stbi_image_free(data);
    return texture;
}