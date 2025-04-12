#pragma once

#include <SDL.h>
#include <string>

class Bullet {
public:
    Bullet(float x, float y, float dirX, float dirY, int damage);
    ~Bullet();

    void setPosition(float x, float y);
    float getX() const;
    float getY() const;
    int getDamage() const;

    bool move(float deltaTime);
    void draw() const;
    void render(SDL_Renderer* renderer);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
private:
    float x, y;          // Position
    int damage;         // Damage dealt by the bullet
    float speed;        // Speed of the bullet
    float dirX, dirY;      // Direction vector (normalized)
    SDL_Texture* texture; // Texture for the bullet sprite

    static const int WIDTH = 10;
    static const int HEIGHT = 10;
};