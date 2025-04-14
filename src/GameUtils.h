#pragma once
#include <SDL.h>
#include <random>

namespace GameUtils {
    // Initialize random number generator
    void initRandom();
    
    // Get a random X position within window bounds
    int getRandomX(int windowWidth, int objectWidth);
    
    // Get a random Y position within window bounds
    int getRandomY(int windowHeight, int objectHeight);
    
    // Get the Y position for ground-based objects
    int getGroundY(int windowHeight, int objectHeight);
    
    // Get the current window dimensions
    void getWindowDimensions(SDL_Window* window, int& width, int& height);
    
    // Get a random boolean value
    bool getRandomBool();
    
    // Get a random number within a range
    float getRandomFloat(float min, float max);
    
    // Collision detection between two rectangles using SDL_Rect
    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
    
    // Check if rectangle is within screen bounds
    bool isInScreen(const SDL_Rect& rect, int screenWidth, int screenHeight);
    
    // Convert position and dimensions to SDL_Rect
    SDL_Rect makeRect(float x, float y, int width, int height);
}