#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>

class Monster {
public:
    virtual ~Monster();

    virtual void update(float deltaTime) = 0;  // Pure virtual function
    virtual void render(SDL_Renderer* renderer) = 0;  // Pure virtual function
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    int getDamage() const { return damage; }
    void takeDamage(int amount) { health -= amount; }
    int getHealth() const { return health; }
    
protected:
    Monster(float x, float y, float w, float h, float maxX, float maxY, int maxHealth, int damage);  // Protected constructor
    
    float x, y;            // Position
    float velX, velY;      // Velocity
    float width, height;  // Dimensions
    float maxX, maxY;
    int health;            // Health points
    bool isJumping;        // Jump state
    int damage;         // Damage dealt by the monster
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, SDL_Texture* texture, const std::string& path);
    
};