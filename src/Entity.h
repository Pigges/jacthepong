#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Entity {
public:
    Entity() = default;
    virtual void update(float delta, std::vector<Entity*> entities) = 0;
    virtual sf::Drawable* getDrawable() = 0;
    virtual sf::FloatRect getGlobalBounds() = 0; // We need the bounds of the entity for collision
    virtual void kill() {
        // By default, an entity doesn't just die. They are immortal.
    }

    // Apparently an entity can be killed ¯\_(ツ)_/¯
    bool isKilled() {
        return killed;
    }

protected:
    bool killed = false;
};