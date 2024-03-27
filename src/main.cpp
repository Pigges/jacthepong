#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"
#include "Player.h"
#include "Ball.h"
#include "RenderManager.h"
#include "ScoreManager.h"
#include "Text.h"
#include "Powerup.h"


int main() {
    // I want this magical clock
    sf::Clock deltaClock;
    // Powerup timer
    sf::Clock powerupClock;

    // Initiate the RenderManager
    RenderManager* renderManager = RenderManager::getInstance();
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Jac The Pong");
    renderManager->setWindow(window);

    // Initiate the ScoreManager
    ScoreManager* scoreManager = ScoreManager::getInstance();
    Text* score = new Text();
    scoreManager->setScore(score);

    // Create the View
    sf::View view;
    view.reset(sf::FloatRect(0, 0, 1280, 720));

    // Store all entities here for rendering
    std::vector<Entity*> entities;

    // Push entities to the vector
    entities.push_back(new Player(1));
    entities.push_back(new Player(2));
    entities.push_back(new Ball());
    entities.push_back(score);


    // The main game loop
    while(window.isOpen()){
        // Delta time to handle framerate independent update
        sf::Time dt = deltaClock.restart();

        // Spawn a new Powerup every once in a while
        if (powerupClock.getElapsedTime().asMilliseconds() >= 10000) {
            powerupClock.restart();
            entities.push_back(new Powerup());
        }

        // Handle Events
        sf::Event event;
        while(window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        // Update
        window.setView(view);
        for (Entity *entity : entities) {
            entity->update(dt.asSeconds(), entities); // Let all entities update

            if (entity->isKilled()) {
                delete entity;
                entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
            }
        }

        // Let a PAUSED game start PLAYING again after a moment
        if (renderManager->getState() == GameState::PAUSED && renderManager->getStateTime() > 1000) renderManager->setState(GameState::PLAYING);


        // Render
        window.clear(sf::Color::Black);

        // Draw every Entity found in entities
        for (Entity *entity : entities) {
            window.draw(*entity->getDrawable()); // Render the Entity's drawable
        }

        window.display(); // Display changes
    }

    // Delete all entities to deallocate memory
    for (Entity *entity : entities) {
        delete entity;
    }

    return 0;
}