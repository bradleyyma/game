#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <memory>
#include <vector>
#include "Bullet.h"

// Forward declaration
class Game;

class Gun {
    public:
        Gun(float x, float y, int damage);
        ~Gun();

        void setPosition(float x, float y);
        int getX() const;
        int getY() const;
        int getDamage() const;
        void shoot(Game & game);
        void update(float deltaTime);
        void render(SDL_Renderer* renderer);

    private:
        float x, y;          // Position
        float fireRate;
        float lastShotTime;  // Time since last shot
        int damage;          // Damage dealt by the gun
};