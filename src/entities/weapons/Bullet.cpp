#include "Bullet.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


Bullet::Bullet(float x, float y, float dirX, float dirY, int damage)
    : x(x), y(y), damage(damage), speed(500.0f), dirX(dirX), dirY(dirY), texture(nullptr) {
    // Load the bullet texture
}

Bullet::~Bullet() {
    // Clean up texture if it exists
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

bool Bullet::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    // Clean up previous texture if it exists
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " 
                  << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!texture) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " 
                  << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return true;
}

void Bullet::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

float Bullet::getX() const {
    return x;
}
float Bullet::getY() const {
    return y;
}

int Bullet::getDamage() const {
    return damage;
}

bool Bullet::move(float deltaTime) {
    x += dirX * speed * deltaTime;
    y += dirY * speed * deltaTime;
    return true;
}

void Bullet::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect renderQuad = {static_cast<int>(x), static_cast<int>(y), WIDTH, HEIGHT};
        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    } else {
        // Fallback: Render a red rectangle if texture not loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect bulletRect = { 
            static_cast<int>(x), 
            static_cast<int>(y), 
            WIDTH, 
            HEIGHT 
        };
        SDL_RenderFillRect(renderer, &bulletRect);
    }
}