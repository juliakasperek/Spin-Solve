#ifndef PHRASELIBRARY_H
#define PHRASELIBRARY_H

#include <string>
#include <vector>

class PhraseLibrary {
public:
    PhraseLibrary();

    std::string getRandomPhrase(const std::string &difficulty);

private:
    std::vector<std::string> easyPhrase;
    std::vector<std::string> hardPhrase;

    int lastEasyIndex;
    int lastHardIndex;
};

#endif // PHRASELIBRARY_H

