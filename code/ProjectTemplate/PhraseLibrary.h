#ifndef PHRASELIBRARY_H
#define PHRASELIBRARY_H

#include <string>
#include <vector>

class PhraseLibrary {
public:
    PhraseLibrary();

    std::string getRandomPhrase(const std::string& difficulty) const;


private:
    std::vector<std::string> easyPhrase;
    std::vector<std::string> hardPhrase;
};

#endif // PHRASELIBRARY_H

