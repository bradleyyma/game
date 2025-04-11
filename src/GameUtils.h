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
} 