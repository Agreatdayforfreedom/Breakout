#include "game_object.h"
#include <string.h>

void Game_Object(GameObject* game_object, vec2 pos, vec2 size, vec2 velocity, vec3 color, Texture2D sprite) {
	game_object->Position[0] = pos[0];
	game_object->Position[1] = pos[1];
	game_object->Size[0] = size[0];
	game_object->Size[1] = size[1];
	game_object->Rotation = 0.0f;
	game_object->Velocity[0] = velocity[0];
	game_object->Velocity[1] = velocity[1];
	game_object->Color[0] = color[0];
	game_object->Color[1] = color[1];
	game_object->Color[2] = color[2];
	game_object->Sprite = sprite;

}

void Draw_GameObject(GameObject* self, SpriteRenderer* renderer) {

	DrawSprite(renderer, self->Sprite, self->Position, self->Size, self->Rotation, self->Color);
}