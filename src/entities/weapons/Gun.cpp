#include "Gun.h"
#include "Bullet.h"
#include "../../Game.h"
#include <iostream>
#include <cmath>

Gun::Gun(float x, float y, int damage)
    : x(x), y(y), damage(damage), fireRate{.2f}, lastShotTime(0.0f) {
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

void Gun::shoot(Game & game) {
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
        auto bullet = std::make_unique<Bullet>(x, y, dirX, dirY, damage);
        game.addBullet(std::move(bullet));
    }
}

void Gun::update(float deltaTime) {
    
}

void Gun::render(SDL_Renderer* renderer) {

}