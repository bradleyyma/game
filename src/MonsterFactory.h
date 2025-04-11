#pragma once

#include <SDL.h>
#include <memory>
#include "Monster.h"
#include "Slime.h"
#include "entities/Hopper.h"

class MonsterFactory {
public:
    enum class MonsterType {
        SLIME,
        HOPPER
    };
    
    static std::unique_ptr<Monster> createMonster(MonsterType type, SDL_Renderer* renderer, int windowWidth, int windowHeight);
};