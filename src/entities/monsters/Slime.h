#pragma once

#include "Monster.h"

class Slime : public Monster {
public:
    Slime(float x, float y, int maxX, int maxY, int direction, float speed);
    // Speed constants
    static const float SLIME_SPEED_MIN;
    static const float SLIME_SPEED_MAX;
    static const float SLIME_SPEED_DEFAULT;
    static const float SLIME_WIDTH;
    static const float SLIME_HEIGHT;
    
    // Health constants
    static const int SLIME_HEALTH;
    static bool loadSharedTexture(SDL_Renderer* renderer, const std::string& path);
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
private:
    int direction;  // Direction of movement
    float speed;       // Current movement speed

    static SDL_Texture* sharedTexture; // Shared texture for all slimes
}; 