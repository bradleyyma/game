#include "Monster.h"
#include <iostream>
#include <random>


Monster::Monster(float x, float y, float maxX, float maxY, int maxHealth, int damage)
    : x{x}, y{y}
    , maxX(maxX), maxY(maxY)
    , health(maxHealth)
    , damage(damage)
    , isJumping(false)
    , texture(nullptr) {
}

Monster::~Monster() {
    // Clean up texture if it exists
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

bool Monster::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Get rid of old loaded surface
    

    return true;
}


void Monster::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect renderQuad = {static_cast<int>(x), static_cast<int>(y), WIDTH, HEIGHT};
        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    } else {
        // Fallback: Render a red rectangle if texture not loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect MonsterRect = { 
            static_cast<int>(x), 
            static_cast<int>(y), 
            WIDTH, 
            HEIGHT 
        };
        SDL_RenderFillRect(renderer, &MonsterRect);
    }
    
    // Render health bar
    int healthBarWidth = WIDTH * health / 100;
    
    // Health bar background (gray)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect healthBg = { 
        static_cast<int>(x), 
        static_cast<int>(y - 10), 
        WIDTH, 
        5 
    };
    SDL_RenderFillRect(renderer, &healthBg);
    
    // Health bar (green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect healthBar = { 
        static_cast<int>(x), 
        static_cast<int>(y - 10), 
        healthBarWidth, 
        5 
    };
    SDL_RenderFillRect(renderer, &healthBar);
}