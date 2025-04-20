#pragma once

#include <SDL.h>
// #include <SDL_image.h>
// #include <string>

class Collidable {
    public:
        virtual SDL_Rect getCollider() const = 0;
        virtual void onCollision(const Collidable& other) = 0;
    
        virtual ~Collidable() = default;
};
            