#pragma once
#include "Collidable.h"
#include <iostream>
#include <vector>

class CollidableManager {
public:
    static void checkCollisions(const std::vector<Collidable*>& objects) {
        bool playerOnPlatform = false;
        bool collisionDetected = false;
        // bool onTopOfPlatform = false;
        for (size_t i = 0; i < objects.size(); ++i) {
            for (size_t j = i + 1; j < objects.size(); ++j) {
                // std::cout << "Object 1 Collider: " << objects[i]->getCollider().x << ", " << objects[i]->getCollider().y << std::endl;
                // std::cout << "Object 2 Collider: " << objects[j]->getCollider().x << ", " << objects[j]->getCollider().y << std::endl;
                if (checkAABBCollision(objects[i]->getCollider(), objects[j]->getCollider())) {
                    objects[i]->onCollision(*objects[j]);
                    objects[j]->onCollision(*objects[i]);
                    collisionDetected = true;
                    // std::cout << "Collision detected between objects " << i << " and " << j << std::endl;
                    Player* player = dynamic_cast<Player*>(objects[i]);
                    Platform* platform = dynamic_cast<Platform*>(objects[j]);
                    if (!player) {
                        player = dynamic_cast<Player*>(objects[j]);
                        platform = dynamic_cast<Platform*>(objects[i]);
                    }
                    
                    if (player && platform){
                        if (player->getCollider().y + player->HEIGHT < platform->getY() + 10) { // Adding 10 to make sure collision direction check isn't super strict
                            playerOnPlatform = true;
                            player->setOnGround(true);
                            player->setPosition(player->getX(), platform->getY() - player->HEIGHT);
                        } else if (player->getY() >= platform->getY() + platform->getHeight() - 10) {
                            player->stopJump();
                            player->setPosition(player->getX(), platform->getY() + platform->getHeight());
                        }
                        // playerOnPlatform = true;
                    }
                }
            }
        }

        for (auto* obj : objects) {
            if (auto* player = dynamic_cast<Player*>(obj)) {
                if (!playerOnPlatform) {
                    std::cout << "resetting jump to true\n" << std::endl;
                    player->resetJump(true);
                    player->setOnGround(false);
                }
            }
        }
    }

private:
    static bool checkAABBCollision(const SDL_Rect& a, const SDL_Rect& b) {
        return SDL_HasIntersection(&a, &b);
    }
};
