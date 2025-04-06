#pragma once

#include <SDL.h>
#include <memory>
#include "Monster.h"
#include "Slime.h"

class MonsterFactory {
public:
    enum class MonsterType {
        SLIME
        // Add more monster types here as they are created
    };
    
    static std::unique_ptr<Monster> createMonster(MonsterType type, SDL_Renderer* renderer, int windowWidth, int windowHeight);
}; 