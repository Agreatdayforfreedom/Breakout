#include "game_level.h"


void Load(GameLevel* self, ResourceManager* RM, const char* file, unsigned int levelWidth, unsigned int levelHeight) {
   
	
    int arr[8][15] = {
        {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        {4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4},
        {4, 1, 4, 1, 4, 0, 0, 1, 0, 0, 4, 1, 4, 1, 4},
        {3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3},
        {3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    };

	init(self,RM, arr, levelWidth, levelHeight);
}

void init(GameLevel *self, ResourceManager* RM, int tileData[8][15], unsigned int levelWidth, unsigned int levelHeight) {

    float unit_width = (float)levelWidth / 15.0f;
    float unit_height = (float)levelHeight / 8.0f;

    int i = 0;
    for (unsigned int y = 0; y < 8; y++)
    {
		for (unsigned int x = 0; x < 15; x++, i++)
		{

            if (tileData[y][x] == 1) // solid
            {
                GameObject go;
                vec2 pos = { unit_width * x, unit_height * y };
                vec2 size = { unit_width, unit_height };
                Game_Object(&go, pos, size,
                     (vec2){0, 0}, (vec3) {
                    0.8f, 0.8f, 0.7f
                },
                        GetTexture(RM, 3)
                );
                go.IsSolid = true;
                go.Destroyed = false;
                self->Bricks[i] = go;

            }
            else if (tileData[y][x] > 1)
            {
                vec3 color = { 1.0f,1.0f,1.0f }; // original: white
                if (tileData[y][x] == 2) {
                    color[0] = 0.2f;
                    color[1] = 0.6f;
                    color[2] = 1.0f;
                }
                else if (tileData[y][x] == 3) {
                    color[0] = 0.0f;
                    color[1] = 0.7f;
                    color[2] = 0.0f;
                }
                else if (tileData[y][x] == 4) {
                    color[0] = 0.8f;
                    color[1] = 0.8f;
                    color[2] = 0.4f;
                }
                else if (tileData[y][x] == 5) {
                    color[0] = 1.0f;
                    color[1] = 0.5f;
                    color[2] = 0.0f;
                }
                
                GameObject go;
                vec2 pos = { unit_width * x, unit_height * y };
                vec2 size = { unit_width, unit_height };
                Game_Object(&go, pos, size, (vec2) { 0, 0 }, color, GetTexture(RM, 2));
                go.IsSolid = false;
                go.Destroyed = false;
                

                self->Bricks[i] = go;
            }
		}
    }
}
void Draw(GameLevel*self, SpriteRenderer *renderer)
{
    for (unsigned int i = 0; i < 8*15; i++)
    {
            if (&self->Bricks[i] != NULL) {

                GameObject tile = self->Bricks[i];
                if (!tile.Destroyed)
                    Draw_GameObject(&tile, renderer);
            }
    }
}