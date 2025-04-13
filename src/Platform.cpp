#include "Platform.h"
#include <iostream>

const float Platform::SPEED = 100.0f;

Platform::Platform() : texture(nullptr), velX(0), velY(0) {
    collider = {0, 0, WIDTH, HEIGHT};
}

void Platform::init(int x, int y, int width, int height) {
    collider = {x, y, width, height};
}

// bool Platform::loadTexture(SDL_Renderer* renderer, const std::string& path) {
//     SDL_Surface* surface = IMG_Load(path.c_str());
//     std::cout << "[Platform] ACTUALLY LOADING: " << path << std::endl;

//     if (!surface) return false;

//     texture = SDL_CreateTextureFromSurface(renderer, surface);
//     SDL_FreeSurface(surface);
//     return texture != nullptr;
// }

bool Platform::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    std::cout << "[Platform] Attempting to load: " << path << std::endl;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    std::cout << "[Platform] Texture loaded successfully!\n";

    return true;
}

void Platform::update(float deltaTime) {
    // If platform needs to move, do it here (optional)
    collider.x += static_cast<int>(velX * deltaTime);
    collider.y += static_cast<int>(velY * deltaTime);
}

void Platform::render(SDL_Renderer* renderer) const {
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &collider);
    } else {
        // Fallback: Render a colored rectangle if texture is missing
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green fallback color
        SDL_RenderFillRect(renderer, &collider);
    }
}

void Platform::setVelocity(float vx, float vy) {
    velX = vx;
    velY = vy;
}

void Platform::reverseX() {
    velX = -velX;
}

bool Platform::checkCollision(const SDL_Rect& other) const {
    return SDL_HasIntersection(&collider, &other);
}

SDL_Rect Platform::getCollider() const {
    return collider;
}

Platform::~Platform() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
