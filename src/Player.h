#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "RenderManager.h"
#include "Powerup.h"


class Player: public Entity {
public:
    Player(const int &player) :
            Entity(), rectShape(sf::Vector2f(30, 200)), player(player) {

        // Position the player (Player 1: Left, Player 2: Right)
        if (player == 1)
            rectShape.setPosition(sf::Vector2(100.0f, renderManager->getWindow().getSize().y / 2 - rectShape.getSize().y / 2));
        else if (player == 2)
            rectShape.setPosition(sf::Vector2(renderManager->getWindow().getSize().x - 130.0f, renderManager->getWindow().getSize().y / 2 - rectShape.getSize().y / 2));
    }

    void update(float delta, std::vector<Entity*> entities) {

        usePowerup();

        // Handle movement up-down (only allow movement within the screen)
        if (sf::Keyboard::isKeyPressed(player == 1 ? sf::Keyboard::W : sf::Keyboard::Up) && rectShape.getPosition().y > 0) {
            rectShape.move(0.f, -500.0f * speedBoost * delta);
        }
        if (sf::Keyboard::isKeyPressed(player == 1 ? sf::Keyboard::S : sf::Keyboard::Down) && rectShape.getPosition().y + rectShape.getSize().y * rectShape.getScale().y < renderManager->getWindow().getSize().y) {
            rectShape.move(0.f, 500.0f * speedBoost * delta);
        }

        // Enforce the boundaries (player outside the screen will be moved back to the edge)
        if (rectShape.getPosition().y < 0) rectShape.move(0.f, -rectShape.getPosition().y);
        if (rectShape.getPosition().y + rectShape.getSize().y * rectShape.getScale().y > renderManager->getWindow().getSize().y) rectShape.move(0.f, -(rectShape.getPosition().y + rectShape.getSize().y * rectShape.getScale().y - renderManager->getWindow().getSize().y));
    };

    sf::Drawable* getDrawable() {
        return &rectShape;
    };

    sf::FloatRect getGlobalBounds() {
        return rectShape.getGlobalBounds();
    }

    void receivePowerup(PowerupType powerup) {
        powerupAge.restart();
        rectShape.setFillColor(sf::Color::Cyan);
        this->powerup = powerup;
    }

    PowerupType getPowerup() {
        return this->powerup;
    }

    // Get what player it is (1 or 2)
    int getPlayer() {
        return this->player;
    }

private:
    sf::RectangleShape rectShape;
    int player;
    float speedBoost = 1;
    sf::Clock powerupAge;
    PowerupType powerup = PowerupType::NONE;

    RenderManager* renderManager = RenderManager::getInstance();

    // Apply the Powerup
    void usePowerup() {

        // Clear the Powerup after a little while
        if (powerupAge.getElapsedTime().asMilliseconds() > 15000) {
            powerup = PowerupType::NONE;
            clearPowerups();
            powerupAge.restart();
            return;
        }
        switch (powerup) {

            case SUPER_SPEED:
                speedBoost = 1.5f;
                break;
            case ELONGATOR:
                rectShape.setScale(sf::Vector2f(1, 1.5));
                break;
            case SUPER_SLOW:
                speedBoost = 2.f/3;
                break;
            case SHRINKINATOR:
                rectShape.setScale(sf::Vector2f(1, 2.f/3));
                break;
            case NONE:
                break;
        }
    }

    // Let's clear the active Powerups!
    void clearPowerups() {
        rectShape.setScale(sf::Vector2f(1, 1));
        speedBoost = 1;
        rectShape.setFillColor(sf::Color::White);
    }
};