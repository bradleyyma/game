#include "Bullet.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

// Initialize static member
SDL_Texture* Bullet::sharedTexture = nullptr;

Bullet::Bullet(float x, float y, float dirX, float dirY, int damage)
    : x(x), y(y), damage(damage), speed(500.0f), dirX(dirX), dirY(dirY) {
}

Bullet::~Bullet() {
    // No need to destroy texture here as it's shared
}

bool Bullet::loadSharedTexture(SDL_Renderer* renderer, const std::string& path) {
    // Clean up previous texture if it exists
    cleanupSharedTexture();

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " 
                  << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture from surface pixels
    sharedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!sharedTexture) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " 
                  << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
    return true;
}

void Bullet::cleanupSharedTexture() {
    if (sharedTexture) {
        SDL_DestroyTexture(sharedTexture);
        sharedTexture = nullptr;
    }
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
    SDL_Rect renderQuad = {static_cast<int>(x), static_cast<int>(y), WIDTH, HEIGHT};
    
    if (sharedTexture) {
        SDL_RenderCopy(renderer, sharedTexture, nullptr, &renderQuad);
    } else {
        // Fallback: Render a colored rectangle if texture not loaded
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow fallback color
        SDL_RenderFillRect(renderer, &renderQuad);
    }
}