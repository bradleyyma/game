#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "entities/monsters/Monster.h"
#include "entities/weapons/Gun.h"
#include "Collidable.h"
#include <iostream>

class Player : public Collidable {
public:
    Player();
    ~Player();

    // Player dimensions
    static const int WIDTH = 50;
    static const int HEIGHT = 50;
    
    void setPosition(float x, float y);
    bool loadTexture(SDL_Renderer* renderer, const std::string& path);
    void handleEvent(SDL_Event& event);
    void update(float deltaTime, Game & game);
    void render(SDL_Renderer* renderer);
    void takeDamage(int amount);
    bool checkCollision(const Monster& monster) const;
    Gun* getGun() { return &gun; } // Accessor for gun
    
    float getX() const { return x; }
    float getY() const { return y; }
    void resetJump(bool jumping);
    void stopJump() { isJumping = true; velY = 0; }
    bool getIsJumping() const { return isJumping; }
    void setOnGround(bool floor) { onGround = floor; }
    bool getOnGround() const { return onGround; }

    SDL_Rect getCollider() const override {
        return collider;
    }

    void onCollision(const Collidable& other) override;
    
private:
    float x, y;            // Position
    float velX, velY;      // Velocity
    int health;            // Health points
    bool isJumping;        // Jump state
    bool onGround;        // Ground state
    Gun gun;
    
    SDL_Texture* texture; // Texture for the player sprite
    SDL_Rect collider;
    
    
    // Movement constants
    static const float SPEED;
    static const float JUMP_VELOCITY;
    static const float GRAVITY;
    
    // Key states
    bool keyStates[4] = {false}; // W, A, S, D
    bool isMouseDown; // Mouse state

};