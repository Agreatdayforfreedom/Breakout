
#include "sprite_renderer.h"


void RenderSprite(SpriteRenderer* self, Shader shader) {
    self->shader = shader;
    initRenderData(self);
}

void DrawSprite(SpriteRenderer* self, Texture2D texture, vec2 position,
    vec2 size, float rotate,
    vec3 color) {
    use(&self->shader);
    mat4 model;

    glm_mat4_identity(model);

    glm_translate(model, (vec3) { position[0], position[1], 0.0f });

    glm_translate(model, (vec3) { 0.5f * size[0], 0.5f * size[1], 0.0f });
    glm_rotate(model, glm_rad(rotate), (vec3) { 0.0f, 0.0f, 1.0f });
    glm_translate(model, (vec3) { -0.5f * size[0], -0.5f * size[1], 0.0f });

    glm_scale(model, (vec3) { size[0], size[1], 1.0f });

    //printf("%f, <==", model[0][0]);
    setMatrix4(&self->shader, "model", model, false);
    
    setVector3f(&self->shader, "spriteColor", color, false);

    glActiveTexture(GL_TEXTURE0);
    //glActiveTexture(GL_TEXTURE1);
    //Bind(&texture);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
    // 
    //printf("\ndraw: %i\n", self->quadVAO);

    glBindVertexArray(self->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void initRenderData(SpriteRenderer* self) {
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
 
    glGenVertexArrays(1, &self->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(self->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
