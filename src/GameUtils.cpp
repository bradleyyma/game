#include "GameUtils.h"
#include <random>

namespace GameUtils {
    // Static random number generator
    static std::mt19937 gen;
    
    void initRandom() {
        std::random_device rd;
        gen.seed(rd());
    }
    
    int getRandomX(int windowWidth, int objectWidth) {
        std::uniform_int_distribution<> dis(0, windowWidth - objectWidth);
        return dis(gen);
    }
    
    int getRandomY(int windowHeight, int objectHeight) {
        std::uniform_int_distribution<> dis(0, windowHeight - objectHeight);
        return dis(gen);
    }
    
    int getGroundY(int windowHeight, int objectHeight) {
        return windowHeight - objectHeight;
    }
    
    void getWindowDimensions(SDL_Window* window, int& width, int& height) {
        SDL_GetWindowSize(window, &width, &height);
    }
    
    bool getRandomBool() {
        std::uniform_int_distribution<> dis(0, 1);
        return dis(gen) == 1;
    }
    
    float getRandomFloat(float min, float max) {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(gen);
    }
} 