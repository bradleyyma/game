#pragma once

#include "Monster.h"

class Slime : public Monster {
public:
    Slime();
    void update(float deltaTime) override;
    
private:
    bool movingRight;  // Direction of movement
    static const float SLIME_SPEED;
}; 