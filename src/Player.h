#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Monster.h"

class Player {
public:
    Player();
    ~Player();
    
    void init(int x, int y);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    void handleEvent(SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void takeDamage(int amount);
    bool checkCollision(const Monster& monster) const;
    
private:
    float x, y;            // Position
    float velX, velY;      // Velocity
    int health;            // Health points
    bool isJumping;        // Jump state
    
    // Texture for the player sprite
    SDL_Texture* texture;
    
    // Player dimensions
    static const int WIDTH = 50;
    static const int HEIGHT = 50;
    
    // Movement constants
    static const float SPEED;
    static const float JUMP_VELOCITY;
    static const float GRAVITY;
    
    // Key states
    bool keyStates[4] = {false}; // W, A, S, D
};