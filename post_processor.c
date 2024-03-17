#include "post_processor.h"

void Post_Processor(PostProcessor* self, Shader shader, unsigned int width, unsigned int height)
{

    self->PostProcessingShader = shader;
    new_Texture2D(&self->Texture);
    self->Width = width;
    self->Height = height;
    self->Confuse = false;
    self->Chaos = false;
    self->Shake = false;

   
    glGenFramebuffers(1, &self->MSFBO);
    glGenFramebuffers(1, &self->FBO);
    glGenRenderbuffers(1, &self->RBO);
    // initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, self->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, self->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height); // allocate storage for render buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, self->RBO); // attach MS render buffer object to framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       printf("ERROR::POSTPROCESSOR: Failed to initialize MSFBO");
    // also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, self->FBO);
    Generate(&self->Texture, width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->Texture.ID, 0); // attach texture to framebuffer as its color attachment
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("ERROR::POSTPROCESSOR: Failed to initialize FBO");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    Generate(&self->Texture, width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->Texture.ID, 0); // attach texture to framebuffer as its color attachment

    initRenderDataPP(self);
    use(&self->PostProcessingShader);
    setInteger(&self->PostProcessingShader, "scene", 0, false);
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right    
    };
    glUniform2fv(glGetUniformLocation(self->PostProcessingShader.ID, "offsets"), 9, (float*)offsets);
    int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(self->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(self->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void BeginRender(PostProcessor* self)
{
    glBindFramebuffer(GL_FRAMEBUFFER, self->MSFBO);
    glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void EndRender(PostProcessor* self)
{
    // now resolve multisampled color-buffer into intermediate FBO to store to texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, self->MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, self->FBO);
    glBlitFramebuffer(0, 0, self->Width, self->Height, 0, 0, self->Width, self->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
}

void RenderPP(PostProcessor* self,float time)
{
    // set uniforms/options
    use(&self->PostProcessingShader);
    setFloat(&self->PostProcessingShader, "time", time, false);
    setInteger(&self->PostProcessingShader, "confuse", self->Confuse, false);
    setInteger(&self->PostProcessingShader, "chaos", self->Chaos, false);
    setInteger(&self->PostProcessingShader, "shake", self->Shake, false);
    // render textured quad
    glActiveTexture(GL_TEXTURE0);
    Bind(&self->Texture);
    glBindVertexArray(self->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void initRenderDataPP(PostProcessor* self)
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &self->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(self->VAO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

