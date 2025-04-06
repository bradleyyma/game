#include "GameUtils.h"
#include <random>

namespace GameUtils {
    int getRandomX(int windowWidth, int objectWidth) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, windowWidth - objectWidth);
        return dis(gen);
    }
    
    int getRandomY(int windowHeight, int objectHeight) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, windowHeight - objectHeight);
        return dis(gen);
    }
    
    int getGroundY(int windowHeight, int objectHeight) {
        return windowHeight - objectHeight;
    }
} 