#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "RenderManager.h"


class Text: public Entity {
public:
    Text() :
            Entity(), score(0, 0) {

        if (!font.loadFromFile("../resources/ubuntu.ttf"))
        {
            // error...
        }

        text.setFont(font);
    }

    void update(float delta, std::vector<Entity*> entities) {
        text.setString(std::to_string(score.x) + " : " + std::to_string(score.y));
        text.setPosition(renderManager->getWindow().getSize().x/2 - text.getLocalBounds().width/2, 0);
    };

    sf::Drawable* getDrawable() {
        return &text;
    };

    sf::FloatRect getGlobalBounds() {
        return text.getGlobalBounds();
    }

    void scorePlayer(int player) {
        player == 1 ? score.x++ : score.y++;
    }

private:
    sf::Font font;
    sf::Text text;
    sf::Vector2<int> score;


    RenderManager* renderManager = RenderManager::getInstance();
};