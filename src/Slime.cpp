#include "Slime.h"
#include "GameUtils.h"
#include <iostream>

// Define static constants
const float Slime::SLIME_SPEED_MIN = 150.0f;
const float Slime::SLIME_SPEED_MAX = 150.0f;
const float Slime::SLIME_SPEED_DEFAULT = 100.0f;
const int Slime::SLIME_HEALTH = 100;

Slime::Slime(int x, int y, int maxX, int maxY, bool moveRight, float speed)
    : Monster(x, y, maxX, maxY, SLIME_HEALTH, 10),
      movingRight(moveRight),
      speed(speed) {
}

void Slime::update(float deltaTime) {
    
    // Move left or right
    if (movingRight) {
        x += speed * deltaTime;
        if (x + WIDTH >= maxX) {
            x = maxX - WIDTH;
            movingRight = false;
        }
    } else {
        x -= speed * deltaTime;
        if (x <= 0) {
            x = 0;
            movingRight = true;
        }
    }
    
    // Keep slime on the ground
    y = maxY - HEIGHT;
}
