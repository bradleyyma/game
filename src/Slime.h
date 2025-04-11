#pragma once

#include "Monster.h"

class Slime : public Monster {
public:
    // Speed constants
    static const float SLIME_SPEED_MIN;
    static const float SLIME_SPEED_MAX;
    static const float SLIME_SPEED_DEFAULT;
    
    // Health constants
    static const int SLIME_HEALTH;
    
    Slime(int x, int y, int maxX, int maxY, bool moveRight, float speed);
    void update(float deltaTime) override;
    
private:
    bool movingRight;  // Direction of movement
    float speed;       // Current movement speed
}; 