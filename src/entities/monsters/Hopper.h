#pragma once

#include "Monster.h"


class Hopper : public Monster {
public:
    // Constructor matching Monster's constructor
    Hopper(int x, int y, int maxX, int maxY);
    virtual ~Hopper() = default;

    void update(float deltaTime) override;

private:
    enum class State {
        Waiting,
        Hopping
    };

    State currentState;
    float waitTime;
    float velY;
    float speed;
    float hopSpeed;
    float elapsedTime;
    int direction; // 1 for right, -1 for left

    static float const GRAVITY;

    void switchDirection();
}; 
