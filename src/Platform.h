#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Platform {
public:
    Platform(int x, int y, int width, int height, int velX, int velY);
    ~Platform();
    
    void init(int x, int y, int width = WIDTH, int height = HEIGHT);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer) const;
    void setVelocity(float vx, float vy);
    void reverseX(); // Bounce logic

    bool checkCollision(const SDL_Rect& other) const;
    SDL_Rect getCollider() const;

private:
    SDL_Texture* texture;
    SDL_Rect collider;     // Position and size
    float velX, velY;      // For moving platforms

    static const int WIDTH = 200;
    static const int HEIGHT = 50;
    static const float SPEED;
};
