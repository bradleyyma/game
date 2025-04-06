#include "Slime.h"
#include <iostream>

// Initialize static member
const float Slime::SLIME_SPEED = 100.0f;  // pixels per second

Slime::Slime() : movingRight(true) {
    // Slime starts moving right by default
}

void Slime::update(float deltaTime) {
    // Move left or right
    velX = movingRight ? SLIME_SPEED : -SLIME_SPEED;
    
    // Update position
    x += velX * deltaTime;
    
    // Get window dimensions
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(SDL_GetRenderer(SDL_GetWindowFromID(1)), &windowWidth, &windowHeight);
    
    // Check if we hit the edge of the screen
    if (x <= 0) {
        x = 0;
        movingRight = true;
    } else if (x >= windowWidth - WIDTH) {
        x = windowWidth - WIDTH;
        movingRight = false;
    }
    
    // Keep slime on the ground
    y = windowHeight - HEIGHT;
    velY = 0;
    isJumping = false;
}
