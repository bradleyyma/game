#include "Player.h"
#include <iostream>

// Initialize static members
const float Player::SPEED = 300.0f;        // pixels per second
const float Player::JUMP_VELOCITY = -400.0f; // pixels per second
const float Player::GRAVITY = 800.0f;      // pixels per second squared

Player::Player()
    : x(0), y(0), velX(0), velY(0), health(100), isJumping(false), texture(nullptr) {
}

Player::~Player() {
    // Clean up texture if it exists
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Player::init(int startX, int startY) {
    x = startX;
    y = startY;
    health = 100;
}

bool Player::loadTexture(SDL_Renderer* renderer, const std::string& path) {
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
    
    // Free the loaded surface since we've created the texture
    SDL_FreeSurface(loadedSurface);
    
    return true;
}

void Player::handleEvent(SDL_Event& event) {
    // Key press
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                keyStates[0] = true;
                break;
            case SDLK_a:
                keyStates[1] = true;
                break;
            case SDLK_s:
                keyStates[2] = true;
                break;
            case SDLK_d:
                keyStates[3] = true;
                break;
            case SDLK_SPACE:
                if (!isJumping) {
                    velY = JUMP_VELOCITY;
                    isJumping = true;
                }
                break;
        }
    }
    // Key release
    else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                keyStates[0] = false;
                break;
            case SDLK_a:
                keyStates[1] = false;
                break;
            case SDLK_s:
                keyStates[2] = false;
                break;
            case SDLK_d:
                keyStates[3] = false;
                break;
        }
    }
}

void Player::update(float deltaTime) {
    // Apply movement based on key states
    velX = 0;
    
    if (keyStates[0]) { // W - up
        velY = -SPEED;
    }
    if (keyStates[1]) { // A - left
        velX = -SPEED;
    }
    if (keyStates[2]) { // S - down
        velY = SPEED;
    }
    if (keyStates[3]) { // D - right
        velX = SPEED;
    }
    
    // Apply gravity if jumping
    if (isJumping) {
        velY += GRAVITY * deltaTime;
    }
    
    // Update position using deltaTime
    x += velX * deltaTime;
    y += velY * deltaTime;
    
    // Simple boundary check (adjust these based on your game window size)
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(SDL_GetRenderer(SDL_GetWindowFromID(1)), &windowWidth, &windowHeight);
    
    if (x < 0) x = 0;
    if (x > windowWidth - WIDTH) x = windowWidth - WIDTH;
    if (y < 0) y = 0;
    
    // Check if player has landed
    if (y >= windowHeight - HEIGHT) {
        y = windowHeight - HEIGHT;
        velY = 0;
        isJumping = false;
    }
}

void Player::render(SDL_Renderer* renderer) {
    if (texture) {
        // Create destination rectangle
        SDL_Rect destRect = { 
            static_cast<int>(x), 
            static_cast<int>(y), 
            WIDTH, 
            HEIGHT 
        };
        
        // Render the sprite
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    } else {
        // Fallback: Render a red rectangle if texture not loaded
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect playerRect = { 
            static_cast<int>(x), 
            static_cast<int>(y), 
            WIDTH, 
            HEIGHT 
        };
        SDL_RenderFillRect(renderer, &playerRect);
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