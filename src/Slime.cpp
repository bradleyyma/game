#include "Slime.h"
#include <iostream>

// Speed constants
const float Slime::SLIME_SPEED_MIN = 150.0f;
const float Slime::SLIME_SPEED_MAX = 150.0f;
const float Slime::SLIME_SPEED_DEFAULT = 100.0f;

const int SLIME_HEALTH = 100;
const int SLIME_DAMAGE = 10;

Slime::Slime(int x, int y, int maxX, int maxY, bool moveRight, float speed)
    : Monster(x, y, maxX, maxY, SLIME_HEALTH, SLIME_DAMAGE)
    , movingRight{moveRight}
    , speed{speed}
{
    // Slime starts moving right by default
}

void Slime::update(float deltaTime) {
    // Move left or right
    velX = movingRight ? speed : -speed;
    
    // Update position
    x += velX * deltaTime;
    
    
    // Check if we hit the edge of the screen
    if (x <= 0) {
        x = 0;
        movingRight = true;
    } else if (x >= maxX - WIDTH) {
        x = maxX - WIDTH;
        movingRight = false;
    }
    
    // Keep slime on the ground
    y = maxY - HEIGHT;
    velY = 0;
    isJumping = false;
}
