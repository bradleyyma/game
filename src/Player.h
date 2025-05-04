#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "entities/monsters/Monster.h"
#include "entities/weapons/Gun.h"

class Player {
public:
    Player();
    ~Player();

    // Player dimensions
    static const int WIDTH = 50;
    static const int HEIGHT = 50;
    
    void setPosition(float x, float y);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    void handleEvent(SDL_Event& event);
    void update(float deltaTime, Game & game);
    void render(SDL_Renderer* renderer);
    void takeDamage(int amount);
    void reset(float reset_x, float reset_y);
    bool checkCollision(const Monster& monster) const;
    Gun* getGun() { return &gun; } // Accessor for gun
    
    float getX() const { return x; }
    float getY() const { return y; }
    int getHealth() const { return health; }
    void resetJump() { isJumping = false; velY = 0; }
    
private:
    float x, y;            // Position
    float velX, velY;      // Velocity
    int health;            // Health points
    bool isJumping;        // Jump state
    Gun gun;
    
    SDL_Texture* texture; // Texture for the player sprite
    
    
    
    // Movement constants
    static const float SPEED;
    static const float JUMP_VELOCITY;
    static const float GRAVITY;
    
    // Key states
    bool keyStates[4] = {false}; // W, A, S, D
    bool isMouseDown; // Mouse state
};