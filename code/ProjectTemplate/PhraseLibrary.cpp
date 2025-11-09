#include "PhraseLibrary.h"

#include <cstdlib>
#include <ctime>

PhraseLibrary::PhraseLibrary() {
    std::srand(std::time(nullptr));

    // EASY PHRASES
    easyPhrase = {
        "LET IT GO",           // Frozen
        "JUST KEEP SWIMMING",  // Finding Nemo
        "TO INFINITY AND BEYOND" // Toy Story
    };

    // HARD PHRASES
    hardPhrase = {
        "I AM YOUR FATHER",                        // Star Wars
        "MAY THE FORCE BE WITH YOU",               // Star Wars
        "LIFE IS LIKE A BOX OF CHOCOLATES"        // Forrest Gump
    };

    lastEasyIndex = -1; // to track last used easy phrase
    lastHardIndex = -1; // to track last used hard phrase
}

std::string PhraseLibrary::getRandomPhrase(const std::string &difficulty) {
    if (difficulty == "easy" && !easyPhrase.empty()) {
        int index;
        do {
            index = std::rand() % easyPhrase.size();
        } while (index == lastEasyIndex && easyPhrase.size() > 1);
        lastEasyIndex = index;
        return easyPhrase[index];
    }
    else if (difficulty == "hard" && !hardPhrase.empty()) {
        int index;
        do {
            index = std::rand() % hardPhrase.size();
        } while (index == lastHardIndex && hardPhrase.size() > 1);
        lastHardIndex = index;
        return hardPhrase[index];
    }
    else {
        return "No Phrases Available";
    }
}
