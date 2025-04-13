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
    
    void init(int x, int y);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    void handleEvent(SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void takeDamage(int amount);
    bool checkCollision(const Monster& monster) const;
    Gun* getGun() { return gun.get(); } // Accessor for gun
    
private:
    float x, y;            // Position
    float velX, velY;      // Velocity
    int health;            // Health points
    bool isJumping;        // Jump state
    std::unique_ptr<Gun> gun;          // Gun as unique pointer
    
    SDL_Texture* texture; // Texture for the player sprite
    
    // Player dimensions
    static const int WIDTH = 50;
    static const int HEIGHT = 50;
    
    // Movement constants
    static const float SPEED;
    static const float JUMP_VELOCITY;
    static const float GRAVITY;
    
    // Key states
    bool keyStates[4] = {false}; // W, A, S, D
    bool isMouseDown; // Mouse state
};