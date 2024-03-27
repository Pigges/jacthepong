#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "Text.h"

class ScoreManager {
private:
    Text* score;
    static ScoreManager* instancePtr;

    ScoreManager() : score(nullptr) {}

public:
    ScoreManager(const ScoreManager &obj) = delete;
    ScoreManager& operator=(const ScoreManager &obj) = delete;

    static ScoreManager *getInstance() {
        // If there is no instance of class
        // then we can create an instance.
        if (instancePtr == NULL) {
            instancePtr = new ScoreManager();
        }

        return instancePtr;
    }

    // Destructor to clean up memory
    ~ScoreManager() {
        if (instancePtr != nullptr) {
            delete instancePtr;
            instancePtr = nullptr;
        }
    }

    void setScore(Text* score) {
        this->score = score;
    }

    Text* getScore() {
        return score;
    }
};
