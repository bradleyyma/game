#include "Gun.h"
#include "Bullet.h"
#include <iostream>
#include <cmath>

Gun::Gun(float x, float y, int damage)
    : x(x), y(y), damage(damage), fireRate{.005f}, activeBullets(), lastShotTime(0.0f) {
}
Gun::~Gun() {
    // Destructor
}
void Gun::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}
int Gun::getX() const {
    return x;
}
int Gun::getY() const {
    return y;
}
int Gun::getDamage() const {
    return damage;
}

void Gun::shoot() {
    float currentTime = SDL_GetTicks() / 1000.0f;
    float deltaTime = currentTime - lastShotTime;
    
    if (deltaTime >= fireRate) {
        // Get mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        // Calculate direction vector
        float dx = mouseX - x;
        float dy = mouseY - y;
        
        // Calculate length for normalization
        float length = sqrt(dx * dx + dy * dy);
        
        // Normalize to get unit vector
        float dirX, dirY;
        if (length > 0) {
            dirX = dx / length;
            dirY = dy / length;
        } else {
            dirX = 0;
            dirY = 0;
        }
        
        lastShotTime = currentTime;
        std::cout << "Gun fired! Direction: (" << dirX << ", " << dirY << ")" << std::endl;
        // Create and store the new bullet
        activeBullets.push_back(std::make_unique<Bullet>(x, y, dirX, dirY, damage));
    }
}

void Gun::update(float deltaTime) {
    // Update all bullets and remove any that are no longer active
    for (auto it = activeBullets.begin(); it != activeBullets.end();) {
        if ((*it)->move(deltaTime)) {
            ++it; // Bullet is still active
        } else {
            it = activeBullets.erase(it); // Remove inactive bullet
        }
    }
}

void Gun::render(SDL_Renderer* renderer) {
    // Render all active bullets
    for (const auto& bullet : activeBullets) {
        bullet->render(renderer);
    }
}