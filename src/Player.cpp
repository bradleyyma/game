#include "Player.h"
#include "Game.h"
#include "Platform.h"
#include <iostream>

// Initialize static members
const float Player::SPEED = 300.0f;        // pixels per second
const float Player::JUMP_VELOCITY = -500.0f; // pixels per second
const float Player::GRAVITY = 800.0f;      // pixels per second squared

Player::Player()
    : x(0), y(0)
    , velX(0), velY(0)
    , collider{0, 0, WIDTH, HEIGHT}
    , health(100), isJumping(true)
    , texture(nullptr), isMouseDown(false)
    , gun(x, y, 30) { // Initialize gun with position and damage;
}

Player::~Player() {
    // Clean up texture if it exists
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Player::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    collider.x = x;
    collider.y = y;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Player::checkCollision(const Monster& monster) const {
    // Simple AABB collision detection
    return (x < monster.getX() + monster.getWidth() &&
            x + WIDTH > monster.getX() &&
            y < monster.getY() + monster.getHeight() &&
            y + HEIGHT > monster.getY());
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
            // case SDLK_w:
            //     keyStates[0] = true;
            //     break;
            case SDLK_a:
                keyStates[1] = true;
                break;
            // case SDLK_s:
            //     keyStates[2] = true;
            //     break;
            case SDLK_d:
                keyStates[3] = true;
                break;
            case SDLK_SPACE:
                std::cout << "Space key pressed!\n";
                // std::cout << "isJumping: " << isJumping << "\n";
                if (!isJumping && onGround) {
                    std::cout << "Player is now jumping!\n";
                    velY = JUMP_VELOCITY;
                    isJumping = true;
                }
                break;
        }
    }
    // Key release
    else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            // case SDLK_w:
            //     keyStates[0] = false;
            //     break;
            case SDLK_a:
                keyStates[1] = false;
                break;
            // case SDLK_s:
            //     keyStates[2] = false;
            //     break;
            case SDLK_d:
                keyStates[3] = false;
                break;
        }
    }
    // Mouse button press
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            // Handle initial mouse click
            isMouseDown = true;
        }
    }
    // Mouse button release
    else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            isMouseDown = false;
        }
    }
}

void Player::update(float deltaTime, Game & game) {
    // Check if mouse is being held down
    if (isMouseDown) {
        // Handle continuous mouse press here
        gun.shoot(game);
    }
    
    // Apply movement based on key states
    velX = 0;
    
    // if (keyStates[0]) { // W - up
    //     velY = -SPEED;
    // }
    if (keyStates[1]) { // A - left
        velX = -SPEED;
    }
    // if (keyStates[2]) { // S - down
    //     velY = SPEED;
    // }
    if (keyStates[3]) { // D - right
        velX = SPEED;
    }
    
    // Apply gravity if jumping
    if (isJumping) {
        std::cout << "Player is still jumping!\n";
        velY += GRAVITY * deltaTime;
    } else {
        velY = 0; // Reset vertical velocity when not jumping
    }
    
    // Update position using deltaTime
    x += velX * deltaTime;
    y += velY * deltaTime;
    collider.x = x;
    collider.y = y;
    
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

    // Update gun position
    gun.setPosition(static_cast<int>(x), static_cast<int>(y));
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

void Player::resetJump(bool jumping) {
    isJumping = jumping;
    if (!isJumping) {
        std::cout << "Player has landed!\n";
        velY = 0;
    }
}

void Player::onCollision(const Collidable& other) { 
    std::cout << "Player Collision Detected!\n";

    if (auto platform = dynamic_cast<const class Platform*>(&other)) {
        if (isJumping) {
            std::cout << "Player collided with platform! Restting jump\n";
            resetJump(false);
        }
    }
}