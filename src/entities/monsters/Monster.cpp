#include "Monster.h"
#include <iostream>
#include <random>


Monster::Monster(float x, float y, float w, float h, float maxX, float maxY, int maxHealth, int damage)
    : x{x}, y{y}
    , width(w), height(h)
    , maxX(maxX), maxY(maxY)
    , health(maxHealth)
    , damage(damage)
    , isJumping(false) {
}

Monster::~Monster() {
}

void Monster::render(SDL_Renderer* renderer, SDL_Texture* texture) {

    if (texture) {
        SDL_Rect renderQuad = {static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height)};
        SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    } else {
        // std::cout << "In monster render" << std::endl;
        // Fallback: Render a red rectangle if texture not loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect MonsterRect = { 
            static_cast<int>(x), 
            static_cast<int>(y), 
            static_cast<int>(width),
            static_cast<int>(height)
        };
        SDL_RenderFillRect(renderer, &MonsterRect);
    }
    
    // Render health bar
    int healthBarWidth = static_cast<int>(width) * health / 100;
    
    // Health bar background (gray)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect healthBg = { 
        static_cast<int>(x), 
        static_cast<int>(y - 10), 
        static_cast<int>(width), 
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

SDL_Texture* Monster::loadTexture(SDL_Renderer* renderer, SDL_Texture* texture,  const std::string& path) {
    // Clean up previous texture if it exists
    std::cout << "[Monster] Attempting to load: " << path << std::endl;
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    
    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " 
                  << IMG_GetError() << std::endl;
        return nullptr;
    }
    
    // Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    // Free the loaded surface since we've created the texture
    SDL_FreeSurface(loadedSurface);
    if (!texture) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " 
                  << SDL_GetError() << std::endl;
        return nullptr;
    }
    
    return texture;
}