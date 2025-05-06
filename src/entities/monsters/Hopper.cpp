#include "Hopper.h"
#include <iostream>
const float Hopper::GRAVITY = 800.0f;
SDL_Texture* Hopper::sharedTexture = nullptr;  // Define the static texture

Hopper::Hopper(float x, float y, int maxX, int maxY)
    : Monster(x, y, WIDTH, HEIGHT, maxX, maxY, 100, 50),  // 100 health, 20 damage
      currentState(State::Waiting),
      waitTime(3.0f),
      hopSpeed(500.0f),
      speed(200.0f),
      elapsedTime(0.0f),
      direction(1)
{
}

void Hopper::update(float deltaTime) {
    elapsedTime += deltaTime;

    switch (currentState) {
        case State::Waiting:
            if (elapsedTime >= waitTime) {
                currentState = State::Hopping;
                velY = -hopSpeed;
                elapsedTime = 0.0f;

            }
            break;

        case State::Hopping:
            std::cout << "Hopper is hopping" << std::endl;
            velY += GRAVITY * deltaTime; // Apply gravity
            x += direction * speed * deltaTime;
            y += velY * deltaTime; // Update vertical position
            // Check boundaries
            if (x <= 0) {
                x = 0;
                switchDirection();
            } else if (x >= maxX - WIDTH) {
                x = maxX - WIDTH;
                switchDirection();
            }
            if (y >= maxY - HEIGHT) {
                y = maxY - HEIGHT;
                velY = 0; // Reset vertical velocity
                currentState = State::Waiting; // Switch back to waiting state
            }
            break;
    }
}

void Hopper::switchDirection() {
    direction *= -1;
}

bool Hopper::loadSharedTexture(SDL_Renderer* renderer, const std::string& path) {
    // Clean up previous texture if it exists
    sharedTexture = loadTexture(renderer, sharedTexture, path);
    if (!sharedTexture) {
        return false;
    }
    return true;
}

void Hopper::render(SDL_Renderer* renderer) {
    Monster::render(renderer, sharedTexture);
}
