#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "RenderManager.h"
#include <cstdlib>
#include <ctime>
#include "RenderManager.h"

enum PowerupType {
        SUPER_SPEED, ELONGATOR, SUPER_SLOW, SHRINKINATOR, NONE
};

// Class to handle different powerups
class Powerup: public Entity {
public:
    Powerup() :
            Entity() {

        std::srand(std::time(0));

        // Choose a random PowerupType
        powerup = static_cast<PowerupType>(rand() % PowerupType::NONE);

        // Grab the appropriate sprite for the Powerup
        switch (powerup) {
            case SUPER_SPEED:
                file = "powerup_superspeed.png";
                break;
            case ELONGATOR:
                file = "powerup_elongator.png";
                break;
            case SUPER_SLOW:
                file = "powerup_superslow.png";
                break;
            case SHRINKINATOR:
                file = "powerup_shrinkinator.png";
                break;
        };

        // Load the powerup sprite
        if (!texture.loadFromFile("../resources/sprites/" + file))
            return;

        // Apply and scale the sprite
        sprite = sf::Sprite(texture);
        sprite.setScale(0.2, 0.2);

        // Scaling animations are cool! (let's start with enlarging)
        animation = 1;

        // Place the Powerup at a random location
        sprite.setPosition(
                rand() % renderManager->getWindow().getSize().x - texture.getSize().x * sprite.getScale().x,
                rand() % renderManager->getWindow().getSize().y - texture.getSize().y * sprite.getScale().y
        );

        // Make sure the Powerup doesn't appear above the screen
        if (sprite.getPosition().y < 0) sprite.setPosition(sprite.getPosition().x, 0);
        // TODO: Handle all cases (maybe force the sprite to be placed between the players)
    }

    void update(float delta, std::vector<Entity*> entities) {
        // Let's die
        if (lifetime.getElapsedTime().asMilliseconds() >= 10000) kill();

        // Hey, make the powerup scale up and down in size. It looks cool!
        sprite.setScale(sprite.getScale().x + 0.00001*animation, sprite.getScale().y + 0.00001*animation);
        if (sprite.getScale().x > 0.22 || sprite.getScale().x < 0.17) animation = -animation;
    };

    sf::Drawable* getDrawable() {
        return &sprite;
    };

    sf::FloatRect getGlobalBounds() {
        return sprite.getGlobalBounds();
    }

    PowerupType getPowerup() {
        return this->powerup;
    }

    // Wow! A Powerup can be killed
    void kill() {
        killed = true;
    }

private:
    sf::Texture texture;
    PowerupType powerup;
    sf::Sprite sprite;
    std::string file;
    int animation;
    sf::Clock lifetime;

    RenderManager* renderManager = RenderManager::getInstance();
};