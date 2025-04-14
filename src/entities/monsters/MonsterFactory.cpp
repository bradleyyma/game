#include "MonsterFactory.h"
#include "../../GameUtils.h"
#include <iostream>

std::unique_ptr<Monster> MonsterFactory::createMonster(MonsterType type, SDL_Renderer* renderer, int windowWidth, int windowHeight) {
    switch (type) {
        case MonsterType::SLIME: {
            int direction = GameUtils::getRandomBool() ? 1 : -1; // Random direction
            auto slime = std::make_unique<Slime>(
                GameUtils::getRandomX(windowWidth, Slime::SLIME_WIDTH),
                GameUtils::getGroundY(windowHeight, Slime::SLIME_HEIGHT),
                windowWidth,
                windowHeight,
                direction,
                GameUtils::getRandomFloat(Slime::SLIME_SPEED_MIN, Slime::SLIME_SPEED_MAX)
            );
            return slime;
        }
        case MonsterType::HOPPER: {
            auto hopper = std::make_unique<Hopper>(
                GameUtils::getRandomX(windowWidth, Hopper::WIDTH),
                GameUtils::getGroundY(windowHeight, Hopper::HEIGHT),
                windowWidth,
                windowHeight
            );
            return hopper;
        }
        default:
            std::cerr << "Unknown monster type!" << std::endl;
            return nullptr;
    }
}