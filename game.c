#include <math.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "sprite_renderer.h"
#include "shader.h"
#include "texture.h"
#include "game_level.h"
#include "post_processor.h"
#include "ball.h"
#include "particle_generator.h"
#include "text_renderer.h"

// Initial velocity of the player paddle
float ShakeTime = 0.0f;
const vec2 PLAYER_SIZE = { 100.0f, 20.0f };
const float PLAYER_VELOCITY = 500.0f;
GameObject Player;

void ConfigGame(Game* self, unsigned int width, unsigned int height)
{
    self->width = width;
    self->height = height;
}



SpriteRenderer spriteRenderer;
ResourceManager RM;
Shader shader;
Shader texture;
Ball ball;
ParticleGenerator particles;
PostProcessor Effects;
TextRenderer Text;

const vec2 INITIAL_BALL_VELOCITY = { 100.0f, -350.0f };
const float BALL_RADIUS = 12.5f;

void Init(Game* self)
{
    LoadShader(&RM, 0);
    LoadShader(&RM, 1);
    LoadShader(&RM, 2);

    // configure shaders
    mat4 projection;
    glm_ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f, projection);
   

    use(&RM.Shaders[0]);
    setInteger(&RM.Shaders[0], "image", 1, false);
    setMatrix4(&RM.Shaders[1], "projection", projection, false);
    use(&RM.Shaders[1]);
    setMatrix4(&RM.Shaders[0], "projection", projection, false);
    // set render-specific controls
    RenderSprite(&spriteRenderer, GetShader(&RM, 0));
    // load textures
    LoadTexture(&RM, "C:/Users/Poe/Downloads/awesomeface.png", true, 0);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/background.jpg", false, 1);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/block.png", false, 2);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/block_solid.png", false, 3);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/paddle.png", true, 4);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/particle.png", true, 5);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/powerup_speed.png", true, 6);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/powerup_sticky.png", true, 7);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/powerup_increase.png", true, 8);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/powerup_confuse.png", true, 9);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/powerup_chaos.png", true, 10);
    LoadTexture(&RM, "C:/Users/Poe/Downloads/powerup_passthrough.png", true, 11);

    self->Lives = 3;
    self->state = GAME_MENU;
    Post_Processor(&Effects, GetShader(&RM, 2), self->width, self->height);



    vec2 playerPos = {
        self->width / 2.0f - PLAYER_SIZE[0] / 2.0f,
        self->height - (int)PLAYER_SIZE[1]
    };

    Game_Object(&Player, playerPos, PLAYER_SIZE, (vec2) { 0.0f, 0.0f }, (vec3) { 1.0f, 1.0f, 1.0f }, GetTexture(&RM, 4));

    GameLevel level1;
    Load(&level1, &RM, "null.txt", self->width, self->height / 2);
    self->levels[0] = level1;
    self->level = 0;
    
    vec2 ballPos; 
    glm_vec2_copy(playerPos, ballPos);
    glm_vec2_add(ballPos, (vec2){PLAYER_SIZE[0] / 2.0f -BALL_RADIUS, -BALL_RADIUS * 2.0f }, ballPos);

    BallObject(&ball, ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, GetTexture(&RM, 0));

    init_particles(&particles);
    Generate_Particle(
        &particles,
        GetShader(&RM, 1),
        GetTexture(&RM, 5),
        500
    );
    init_TextRenderer(&Text, &RM, self->width, self->height);
    LoadText(&Text,"C:/Users/Poe/Downloads/OCRAEXT.TTF", 24);

}

void Update(Game* self, float dt)
{
    Move(&ball, dt, self->width);
    Update_Particles(&particles, dt, &ball, 2, (vec2){ ball.Radius / 2.0f });
    UpdatePowerUps(self, dt);
    if (ShakeTime > 0.0f)
    {
        //printf("%i, %i\n", *&ball.inherit.Position[1], self->height);

        
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects.Shake = false;
    }
    if (ball.inherit.Position[1] >= self->height) // did ball reach bottom edge?
    {
        self->Lives = self->Lives - 1;
        ResetPlayer(self);
        //printf("%i\n", self->Lives);

        // did the player lose all his lives? : Game over
        if (self->Lives == 0)
        {
            ResetLevel(self);
            self->state = GAME_MENU;
        }
    }
    DoCollisions(self);
}

void Render(Game* self)
{
    if (self->state == GAME_ACTIVE || self->state == GAME_MENU || self->state == GAME_WIN)
    {
        Texture2D texture = GetTexture(&RM, 0);
        Texture2D texture_bg = GetTexture(&RM, 1);
        //self->Lives = 3;

        BeginRender(&Effects);

        DrawSprite(&spriteRenderer, GetTexture(&RM, 1), (vec2) { 0.0f, 0.0f }, (vec2) { self->width, self->height }, 0.0f, (vec3) { 1.0f, 1.0f, 1.0f });
        Draw(&self->levels[self->level], &spriteRenderer);
        Draw_Particles(&particles);
        Draw_GameObject(&Player, &spriteRenderer);
        Draw_GameObject(&ball, &spriteRenderer);
        //TODO

        for (unsigned int i = 0; i < 6; i++)
        {
            PowerUp powerUp = self->powerUps[i];
            // 
            //if (powerUp != NULL) {
            if (!powerUp.inherit.Destroyed) {
                Draw_GameObject(&powerUp, &spriteRenderer);
            }
            //}

        }

        EndRender(&Effects);
        RenderPP(&Effects, glfwGetTime());

    }
    if (self->state == GAME_MENU) {
        RenderText(&Text, "Press ENTER to start", 250.0f, self->height / 2.0f, 1.0f, (vec3) { 1.0f, 1.0f, 1.0f });
    }
    char str[24] = "Lives: ";
    char lives[33];
    itoa(self->Lives, lives, 10);
    strcat(str, lives);
    RenderText(&Text, str, 5.0f, 5.0f, 1.0f, (vec3) { 1.0f, 1.0f, 1.0f });
    
}

void ProcessInput(Game* self, float dt)
{
    if (self->state == GAME_MENU)
    {
        if (self->keys[GLFW_KEY_ENTER])
        {
            self->state = GAME_ACTIVE;
        }
        /*if (self->keys[GLFW_KEY_W] )
        {
            self->level = (self->level + 1) % 4;
        }
        if (self->keys[GLFW_KEY_S] )
        {
            if (self->level > 0)
                --self->level;
            else
                self->level = 3;
        }*/
    }
    if (self->state == GAME_ACTIVE) {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (self->keys[GLFW_KEY_A])
        {
            if (Player.Position[0] >= 0.0f) {
                Player.Position[0] -= velocity;
                if (ball.Stuck)
                    ball.inherit.Position[0] -= velocity;
            }
        }
        if (self->keys[GLFW_KEY_D])
        {
            if (Player.Position[0] <= self->width - Player.Size[0]) {

                Player.Position[0] += velocity;
                if (ball.Stuck)
                    ball.inherit.Position[0] += velocity;
            }
        }
        if (self->keys[GLFW_KEY_SPACE])
            ball.Stuck = false;
    }
}

void ResetLevel(Game* self)
{
    self->Lives = 3;
    GameLevel level1;
    Load(&level1, &RM, "null.txt", self->width, self->height / 2);
    self->levels[0] = level1;
    self->level = 0;
}

void ResetPlayer(Game* self)
{

    Player.Size[0] = PLAYER_SIZE[0];
    Player.Size[1] = PLAYER_SIZE[1];
    Player.Position[0] = self->width / 2.0f - PLAYER_SIZE[0] / 2.0f, 
    Player.Position[1] = self->height - PLAYER_SIZE[1];
    Reset(&ball, (vec2){ Player.Position[0] + PLAYER_SIZE[0] / 2.0f - BALL_RADIUS, Player.Position[1] - (BALL_RADIUS * 2.0f) }, INITIAL_BALL_VELOCITY);
    Effects.Chaos = Effects.Confuse = false;
    ball.PassThrough = ball.Sticky = false;
    Player.Color[0] = 1.0f;
    Player.Color[1] = 1.0f;
    Player.Color[2] = 1.0f;
    ball.inherit.Color[0] = 1.0f;
    ball.inherit.Color[1] = 1.0f;
    ball.inherit.Color[2] = 1.0f;
}

bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}
void SpawnPowerUps(Game* self, GameObject* block)
{
    PowerUp powerUp;
    if (ShouldSpawn(75)) {
        
        new_PowerUp(&powerUp, "speed", (vec3){ 0.5f, 0.5f, 1.0f }, 0.0f, block->Position, GetTexture(&RM, 6));
        printf("speed: &texture: %i\n x: % f, y : % f\n", powerUp.inherit.Sprite.ID, powerUp.inherit.Position[0], powerUp.inherit.Position[1]);
        self->powerUps[0] = powerUp;
    }
   if (ShouldSpawn(75)) {
        new_PowerUp(&powerUp, "sticky", (vec3) { 1.0f, 0.5f, 1.0f }, 20.0f, block->Position, GetTexture(&RM, 7));
        self->powerUps[1] = powerUp;
    }
    if (ShouldSpawn(75)) {
        
        new_PowerUp(&powerUp, "pad-size-increase", (vec3) { 1.0f, 0.6f, 0.4 }, 0.0f, block->Position, GetTexture(&RM, 9));
        self->powerUps[3] = powerUp;
    }
    if (ShouldSpawn(75)) {
        new_PowerUp(&powerUp, "pass-through", (vec3) { 0.5f, 1.0f, 0.5f }, 10.0f, block->Position, GetTexture(&RM, 8));
        self->powerUps[2] = powerUp;
        
    }
    if (ShouldSpawn(15)) {
        new_PowerUp(&powerUp, "confuse", (vec3) { 1.0f, 0.3f, 0.3f }, 15.0f, block->Position, GetTexture(&RM, 10));
        self->powerUps[4] = powerUp;
        
    }
    if (ShouldSpawn(15)) {
        new_PowerUp(&powerUp, "chaos", (vec3) { 0.9f, 0.25f, 0.25f }, 15.0f, block->Position, GetTexture(&RM, 11));
        self->powerUps[5] = powerUp;
    }
}

void UpdatePowerUps(Game* self, float dt)
{
    for (unsigned int i = 0; i < 6; i++)
    {
        PowerUp* powerUp = &self->powerUps[i];
        //printf("%f, vel<\n", powerUp.inherit.Velocity[1]);

        powerUp->inherit.Position[0] += powerUp->inherit.Velocity[0] * dt;
        powerUp->inherit.Position[1] += powerUp->inherit.Velocity[1] * dt;
        
        if (powerUp->Activated)
        {
            powerUp->Duration -= dt;
            if (powerUp->Duration <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp->Activated = false;
                // deactivate effects
                if (powerUp->Type == "sticky")
                {
                    if (!isOtherPowerUpActive(self->powerUps, "sticky"))
                    {	// only reset if no other PowerUp of type sticky is active
                        ball.Sticky = false;
                        glm_vec3_copy((vec3) {1.0f,1.0f,1.0f}, Player.Color);
                    }
                }
                else if (powerUp->Type == "pass-through")
                {
                    if (!isOtherPowerUpActive(self->powerUps, "pass-through"))
                    {	// only reset if no other PowerUp of type pass-through is active
                        ball.PassThrough = false;
                        glm_vec3_copy((vec3) { 1.0f, 1.0f, 1.0f }, ball.inherit.Color);
                    }
                }
                else if (powerUp->Type == "confuse")
                {
                    if (!isOtherPowerUpActive(self->powerUps, "confuse"))
                    {	// only reset if no other PowerUp of type confuse is active
                        Effects.Confuse = false;
                    }
                }
                else if (powerUp->Type == "chaos")
                {
                    if (!isOtherPowerUpActive(self->powerUps, "chaos"))
                    {	// only reset if no other PowerUp of type chaos is active
                        Effects.Chaos = false;
                    }
                }
            }
        }
    }
    //TODO:
    /*this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());*/
}

void DoCollisions(Game*self)
{
    
    for (unsigned int i = 0; i < 8 * 15; i++)
    {
        GameObject *box = &self->levels[self->level].Bricks[i];
        if (!box->Destroyed)
        {
            Collision collision = CheckCollision(&ball, box);
            if (collision.collide)
            {
                if (!box->IsSolid) {
                    box->Destroyed = true;
                    SpawnPowerUps(self, box);
                }
                else {
                    ShakeTime = 0.05f;
                    Effects.Shake = true;
                }

                // collision resolution
                Direction dir = collision.direction;
                if(!(ball.PassThrough && !box->IsSolid))
                if (dir == LEFT || dir == RIGHT) // horizontal collision
                {
                    ball.inherit.Velocity[0] = -ball.inherit.Velocity[0]; // reverse horizontal velocity
                    // relocate
                    float penetration = ball.Radius - abs(collision.dv[0]);
                    if (dir == LEFT)
                        ball.inherit.Position[0] += penetration; // move ball to right
                    else
                        ball.inherit.Position[0] -= penetration; // move ball to left;
                }
                else // vertical collision
                {
                    //ball.inherit.Velocity[1] = -1.0f * abs(ball.inherit.Velocity[1]); // reverse vertical velocity
                    ball.inherit.Velocity[1] = -ball.inherit.Velocity[1];
                     //relocate
                    float penetration = ball.Radius - abs(collision.dv[1]);
                    if (dir == UP)
                        ball.inherit.Position[1] += penetration; // move ball back up
                    else
                        ball.inherit.Position[1] -= penetration; // move ball back down
                }
            }
        }
    }

    Collision result = CheckCollision(&ball, &Player);
    if (!ball.Stuck && result.collide)
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Player.Position[0] + Player.Size[0] / 2.0f;
        float distance = (ball.inherit.Position[0] + ball.Radius) - centerBoard;
        float percentage = distance / (Player.Size[0] / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        vec2 oldVelocity;
        glm_vec2_copy(ball.inherit.Velocity, oldVelocity);
        ball.inherit.Velocity[0] = INITIAL_BALL_VELOCITY[0] * percentage * strength;
        ball.inherit.Velocity[1] = -ball.inherit.Velocity[1];

        vec2 norm;
        vec2 mult;
        glm_vec2_normalize_to(ball.inherit.Velocity, norm);
        glm_vec2_scale(norm, glm_vec2_norm(oldVelocity), mult);
        glm_vec2_copy(mult, ball.inherit.Velocity);

        ball.Stuck = ball.Sticky;
    }

    for (unsigned int i = 0; i < 6; i++)
    {
        PowerUp* powerUp = &self->powerUps[i];
        if (!powerUp->inherit.Destroyed)
        {
            if (powerUp->inherit.Position[1] >= self->height)
                powerUp->inherit.Destroyed = true;
            if (CheckCollision_AABB(&Player, powerUp))
            {	// collided with player, now activate powerup
                ActivatePowerUp(powerUp);
                powerUp->inherit.Destroyed = true;

                powerUp->Activated = true;
            }
        }
    }

}

void ActivatePowerUp(PowerUp* powerUp)
{
    if (powerUp->Type == "speed")
    {
        ball.inherit.Velocity[0] *= 1.2f;
        ball.inherit.Velocity[1] *= 1.2f;
    }
    else if (powerUp->Type == "sticky")
    {
        ball.Sticky = true;
        glm_vec3_copy((vec3){ 1.0f, 0.5f, 1.0f }, Player.Color);
    }
    else if (powerUp->Type == "pass-through")
    {
        ball.PassThrough = true;
        glm_vec3_copy((vec3) { 1.0f, 0.5f, 0.5f }, ball.inherit.Color);
    }
    else if (powerUp->Type == "pad-size-increase")
    {
        Player.Size[0] += 50.0f;
    }
    else if (powerUp->Type == "confuse")
    {
        if (!Effects.Chaos)
            Effects.Confuse = true; // only activate if chaos wasn't already active
    }
    else if (powerUp->Type == "chaos")
    {
        if (!Effects.Confuse)
            Effects.Chaos = true;
    }
}

bool isOtherPowerUpActive(PowerUp powerUps[], char* type)
{
    //TODO
    for (unsigned int i = 0; i < 6; i++)
    {
        PowerUp* powerUp = &powerUps[i];
        if (powerUp->Activated)
            if (powerUp->Type == type)
                return true;
    }
    return false;
}

Direction VectorDirection(vec2 target)
{
    vec2 compass[] = {
        {0.0f, 1.0f},	// up
        {1.0f, 0.0f},	// right
        {0.0f, -1.0f},	// down
        {-1.0f, 0.0f}	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        vec2 normalized; 
        glm_vec2_normalize_to(target, normalized);
        float dot_product = glm_vec2_dot(normalized, compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

bool CheckCollision_AABB(GameObject* one, GameObject* two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one->Position[0] + one->Size[0] >= two->Position[0] &&
        two->Position[0] + two->Size[0] >= one->Position[0];
    // collision y-axis?
    bool collisionY = one->Position[1] + one->Size[1] >= two->Position[1] &&
        two->Position[1] + two->Size[1] >= one->Position[1];
    // collision only if on both axes
    return collisionX && collisionY;
}

Collision CheckCollision(Ball *one, GameObject *two) 
{
    //todo
    //vec2 center = { one->inherit.Position[0] + one->Radius, one->inherit.Position[1] + one->Radius };;
    ////glm_vec2_adds(one->inherit.Position, one->Radius, center);

    //vec2 aabb_half_extents = { two->Size[0] / 2.0f, two->Size[1] / 2.0f };
    //vec2 aabb_center = {
    //    two->Position[0] + aabb_half_extents[0],
    //    two->Position[1] + aabb_half_extents[1]
    //};

    //vec2 difference;

    //glm_vec2_sub(center, aabb_center, difference);

    //vec2 clamped;
    //glm_vec2_copy(difference, clamped);
    //vec2 negated;  
    //glm_vec2_negate_to(aabb_half_extents, negated);
    //float min = glm_vec2_min(negated);


    //float max = glm_vec2_max(aabb_half_extents);
    //glm_vec2_clamp(clamped,min, max);

    //vec2 closest;
    //glm_vec2_add(aabb_center, clamped, closest);
    //glm_vec2_sub(closest, center, difference);

    bool collisionX = one->inherit.Position[0] + one->inherit.Size[0] >= two->Position[0] &&
        two->Position[0] + two->Size[0] >= one->inherit.Position[0];
    // collision y-axis?
    bool collisionY = one->inherit.Position[1] + one->inherit.Size[1] >= two->Position[1] &&
        two->Position[1] + two->Size[1] >= one->inherit.Position[1];
    // collision only if on both axes
    Collision collision;
    if (collisionX && collisionY) {
        collision.collide = true;
        collision.direction = VectorDirection((vec2) { 0.0f, 0.0f });
        glm_vec2_copy((vec2){0.0f,0.0f}, collision.dv);
    }
    else {
        collision.collide = false;
        collision.direction = UP;
        glm_vec2_copy((vec2) { 0.0f, 0.0f }, collision.dv);
    }
    return collision;
    
   
}

