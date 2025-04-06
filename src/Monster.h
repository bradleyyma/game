#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>

class Monster {
public:
    // Monster dimensions
    static const int WIDTH = 50;
    static const int HEIGHT = 50;
    
    virtual ~Monster();
    
    void init(int x, int y);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    virtual void update(float deltaTime) = 0;  // Pure virtual function
    void render(SDL_Renderer* renderer);
    float getX() const { return x; }
    float getY() const { return y; }
    
protected:
    Monster();  // Protected constructor
    
    float x, y;            // Position
    float velX, velY;      // Velocity
    int health;            // Health points
    bool isJumping;        // Jump state
    
    // Texture for the Monster sprite
    SDL_Texture* texture;
    
    // Movement constants
    static const float SPEED;
    static const float JUMP_VELOCITY;
    static const float GRAVITY;
    
    // Key states
    bool keyStates[4] = {false}; // W, A, S, D
};