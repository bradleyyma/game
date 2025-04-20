#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Collidable.h"
#include <iostream>

class Platform : public Collidable {
public:
    Platform(int x, int y, int width, int height, int velX, int velY);
    ~Platform();
    
    void init(int x, int y, int width = WIDTH, int height = HEIGHT);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer) const;
    void setVelocity(float vx, float vy);
    void reverseX(); // Bounce logic

    int getX() const { return collider.x; }
    int getY() const { return collider.y; }
    int getHeight() const { return HEIGHT; }
    int getWidth() const { return WIDTH; }

    bool checkCollision(const SDL_Rect& other) const;
    SDL_Rect getCollider() const override;

    void onCollision(const Collidable& other) override;

private:
    SDL_Texture* texture;
    SDL_Rect collider;     // Position and size
    float velX, velY;      // For moving platforms

    static const int WIDTH = 200;
    static const int HEIGHT = 50;
    static const float SPEED;
};
