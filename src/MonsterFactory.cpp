#include "MonsterFactory.h"
#include "GameUtils.h"
#include <iostream>

std::unique_ptr<Monster> MonsterFactory::createMonster(MonsterType type, SDL_Renderer* renderer, int windowWidth, int windowHeight) {
    switch (type) {
        case MonsterType::SLIME: {
            auto slime = std::make_unique<Slime>(
                GameUtils::getRandomX(windowWidth, Monster::WIDTH),
                GameUtils::getGroundY(windowHeight, Monster::HEIGHT),
                windowWidth,
                windowHeight,
                GameUtils::getRandomBool(),
                GameUtils::getRandomFloat(Slime::SLIME_SPEED_MIN, Slime::SLIME_SPEED_MAX)
            );
            slime->loadTexture(renderer, "../assets/images/slime.png");
            return slime;
        }
        default:
            std::cerr << "Unknown monster type!" << std::endl;
            return nullptr;
    }
} 