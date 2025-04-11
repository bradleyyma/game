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

    virtual void update(float deltaTime) = 0;  // Pure virtual function

    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    void render(SDL_Renderer* renderer);
    float getX() const { return x; }
    float getY() const { return y; }
    int getDamage() const { return damage; }
    
protected:
    Monster(int x, int y, int maxX, int maxY, int maxHealth, int damage);  // Protected constructor
    
    float x, y;            // Position
    float velX, velY;      // Velocity
    float maxX, maxY;
    int health;            // Health points
    bool isJumping;        // Jump state
    int damage;         // Damage dealt by the monster
    
    // Texture for the Monster sprite
    SDL_Texture* texture;
    
};