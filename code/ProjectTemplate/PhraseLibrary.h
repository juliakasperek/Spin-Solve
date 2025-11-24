#ifndef PHRASELIBRARY_H
#define PHRASELIBRARY_H

#include <string>
#include <vector>

struct Phrase {
    std::string text;
    std::string category;
};

class PhraseLibrary {
public:
    PhraseLibrary();

    Phrase getRandomPhrase(const std::string &difficulty);

private:
    std::vector<Phrase> easyPhrase;
    std::vector<Phrase> hardPhrase;

    int lastEasyIndex;
    int lastHardIndex;
};

#endif // PHRASELIBRARY_H

