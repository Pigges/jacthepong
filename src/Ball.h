#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "Entity.h"
#include "RenderManager.h"
#include "Player.h"
#include "ScoreManager.h"
#include "Powerup.h"

class Ball: public Entity {
public:
    Ball() :
            Entity(), circShape(15.0f), velocity(0, 0) {
        circShape.setPosition(sf::Vector2(renderManager->getWindow().getSize().x / 2 - circShape.getRadius(), renderManager->getWindow().getSize().y / 2 - circShape.getRadius()));

        // Time for some randomness
        std::srand(std::time(0));

        // Set a random direction of the velocity
        velocity.y = 500.f * ((rand() % 2 == 0) ? 1 : -1);
        velocity.x = 500.f * ((rand() % 2 == 0) ? 1 : -1);
    }

    void update(float delta, std::vector<Entity*> entities) {
        // Handle bouncing on the walls
        if (circShape.getPosition().y < 0) {
            velocity.y *= -1;
            circShape.setPosition(circShape.getPosition().x, circShape.getPosition().y < 0);
        }
        if (circShape.getPosition().y > renderManager->getWindow().getSize().y - circShape.getRadius()*2) {
            velocity.y *= -1;
            circShape.setPosition(circShape.getPosition().x, renderManager->getWindow().getSize().y - circShape.getRadius()*2);
        }

        // Handle collisions
        for (Entity *entity : entities) {
            // Reverse direction if the ball hit a player
            if (typeid(*entity) == typeid(Player))
                if (circShape.getGlobalBounds().intersects(entity->getGlobalBounds())) velocity.x *= -1;

            // Handle catching a Powerup
            if (typeid(*entity) == typeid(Powerup) && circShape.getGlobalBounds().intersects(entity->getGlobalBounds())) {
                entity->kill(); // We are evil and need to kill the Powerup

                // Let's treat the entity as a Powerup (inappropriate to discriminate against a Powerups)
                auto* powerup = dynamic_cast<Powerup*>(entity);

                Player* player;

                // Find the appropriate player to send the Powerup to (negative Powerups are sent to the opposite player)
                switch (powerup->getPowerup()) {
                    case SUPER_SPEED:
                    case ELONGATOR:
                        player = getplayer(velocity.x > 0 ? 1 : 2, entities);
                        break;
                    case SUPER_SLOW:
                    case SHRINKINATOR:
                        player = getplayer(velocity.x > 0 ? 2 : 1, entities);
                        break;
                    case NONE:
                        return;
                        break;
                };

                // Maybe we didn't find a player and therefore couldn't give the powerup (why doesn't player 1 or 2 exist?)
                if (player == nullptr) {
                    std::cout << "Error: couldn't give player powerup because player wasn't found!";
                    return;
                }

                // Give player the Powerup
                player->receivePowerup(powerup->getPowerup());
            }
        }

        // Handle points
        if (circShape.getPosition().x > renderManager->getWindow().getSize().x || circShape.getPosition().x + circShape.getRadius()*2 < 0) {
            scoreManager->getScore()->scorePlayer(velocity.x > 0 ? 1 : 2); // Give a point to the winning player

            // Place ball back to middle and reverse its direction
            circShape.setPosition(sf::Vector2(renderManager->getWindow().getSize().x / 2 - circShape.getRadius(), renderManager->getWindow().getSize().y / 2 - circShape.getRadius()));
            velocity.x *= -1;

            // Pause the game
            renderManager->setState(GameState::PAUSED);
        }

        // Move the ball
        if (renderManager->getState() == GameState::PLAYING) circShape.move(velocity.x * delta, velocity.y * delta);
    };

    sf::Drawable* getDrawable() {
        return &circShape;
    };

    sf::FloatRect getGlobalBounds() {
        return circShape.getGlobalBounds();
    }

private:
    sf::CircleShape circShape;
    sf::Vector2f velocity;

    // Access singleton objects
    RenderManager* renderManager = RenderManager::getInstance();
    ScoreManager* scoreManager = ScoreManager::getInstance();

    // Find the player among the enteties and returns
    Player* getplayer(int id, std::vector<Entity*> entities) {
        for (Entity *entity : entities) {
            if (typeid(*entity) == typeid(Player)) {
                auto* player = dynamic_cast<Player*>(entity);
                if (player->getPlayer() == id) return player;
            }
        }
        return nullptr;
    }
};