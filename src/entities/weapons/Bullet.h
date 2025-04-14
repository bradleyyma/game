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
    void render(SDL_Renderer* renderer);
    
    static bool loadSharedTexture(SDL_Renderer* renderer, const std::string& path);
    static void cleanupSharedTexture();

    static const int WIDTH = 10;
    static const int HEIGHT = 10;

private:
    float x, y;          // Position
    int damage;         // Damage dealt by the bullet
    float speed;        // Speed of the bullet
    float dirX, dirY;      // Direction vector (normalized)
    static SDL_Texture* sharedTexture; // Shared texture for all bullets
};