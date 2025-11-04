#include "PhraseLibrary.h"


PhraseLibrary::PhraseLibrary() {


    easyPhrase = {



    };

    hardPhrase = {



    };
}

    std::string PhraseLibrary::getRandomPhrase(const std::string& difficulty) const {

        if (difficulty == "easy" && !easyPhrase.empty()) {

        }
        else if (difficulty == "hard" && !hardPhrase.empty()) {

        }
        else {
            return "No Phrases Available";
        }
}
