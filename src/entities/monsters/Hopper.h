#pragma once

#include "Monster.h"


class Hopper : public Monster {
public:
    // Constructor matching Monster's constructor
    Hopper(float x, float y, int maxX, int maxY);
    virtual ~Hopper() = default;

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    static bool loadSharedTexture(SDL_Renderer* renderer, const std::string& path);

    static constexpr float WIDTH { 50.0f };
    static constexpr float HEIGHT { 50.0f };

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
    static SDL_Texture * sharedTexture; // Shared texture for all hoppers

    static float const GRAVITY;

    void switchDirection();
}; 
