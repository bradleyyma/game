#pragma once

namespace GameUtils {
    // Get a random X position within window bounds
    int getRandomX(int windowWidth, int objectWidth);
    
    // Get a random Y position within window bounds
    int getRandomY(int windowHeight, int objectHeight);
    
    // Get the Y position for ground-based objects
    int getGroundY(int windowHeight, int objectHeight);
} 