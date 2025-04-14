#include "Slime.h"
#include "../../GameUtils.h"
#include <iostream>

// Define static constants
const float Slime::SLIME_SPEED_MIN = 150.0f;
const float Slime::SLIME_SPEED_MAX = 150.0f;
const float Slime::SLIME_SPEED_DEFAULT = 100.0f;
const int Slime::SLIME_HEALTH = 100;
const float Slime::SLIME_WIDTH = 55.0f;
const float Slime::SLIME_HEIGHT = 40.0f;
SDL_Texture* Slime::sharedTexture = nullptr;  // Define the static texture

Slime::Slime(float x, float y, int maxX, int maxY, int direction, float speed)
    : Monster(x, y, SLIME_WIDTH, SLIME_HEIGHT, maxX, maxY, SLIME_HEALTH, 10)
    , speed(speed), direction(direction) {

}

void Slime::update(float deltaTime) {
    
    // Move left or right
 
    x += direction * speed * deltaTime;
    if (x + width >= maxX) {
        x = maxX - width;
        direction = -1; // Change direction to left
    }else if (x <= 0) {
        x = 0;
        direction = 1;
    }

}

bool Slime::loadSharedTexture(SDL_Renderer* renderer, const std::string& path) {
    // Clean up previous texture if it exists
    sharedTexture = loadTexture(renderer, sharedTexture, path);
    if (!sharedTexture) {
        std::cerr << "Failed to load slime texture!" << std::endl;
        return false;
    }
    std::cout << "[Slime] Texture loaded successfully!\n";
    return true;
}

void Slime::render(SDL_Renderer* renderer) {
    std::cout<< "[Slime] Rendering Slime" << std::endl;
    Monster::render(renderer, sharedTexture);
}
