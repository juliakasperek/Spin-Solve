#include "PhraseLibrary.h"

#include <cstdlib>
#include <ctime>

PhraseLibrary::PhraseLibrary() : lastEasyIndex(-1), lastHardIndex(-1) {
    std::srand(std::time(nullptr));

    // EASY PHRASES
    easyPhrase = {
        // FOOD
        {"CHOCOLATE CAKE", "Food"},
        {"APPLE PIE", "Food"},
        {"PEPPERONI PIZZA", "Food"},
        {"MAC AND CHEESE", "Food"},
        {"STRAWBERRY JAM", "Food"},
        {"CHOCOLATE CHIP COOKIE", "Food"},
        {"FRUIT SALAD", "Food"},

        // ANIMAL
        {"POLAR BEAR", "Animal"},
        {"ORANGE CAT", "Animal"},
        {"BLUE WHALE", "Animal"},
        {"GRAY WOLF", "Animal"},
        {"SEA TURTLE", "Animal"},
        {"GOLDEN RETRIEVER", "Animal"},

        // MOVIE TITLE
        {"TOY STORY", "Movie Title"},
        {"JURASSIC PARK", "Movie Title"},
        {"HARRY POTTER", "Movie Title"},
        {"TANGLED", "Movie Title"},
        {"THE LION KING", "Movie Title"},
        {"FINDING NEMO", "Movie Title"},
        {"MONSTERS INC", "Movie Title"},
        {"THE LITTLE MERMAID", "Movie Title"},

        // SONG TITLE
        {"SHAKE IT OFF", "Song Title"},
        {"COUNTING STARS", "Song Title"},
        {"SHAPE OF YOU", "Song Title"},
        {"FIREWORK", "Song Title"},
        {"LET IT GO", "Song Title"},
        {"BLANK SPACE", "Song Title"},
        {"HEY BROTHER", "Song Title"},
        {"DON'T YOU WORRY CHILD", "Song Title"},
        {"UNDER CONTROL", "Song Title"},
        {"MIRACLE MAKER", "Song Title"},
        {"HAVANA", "Song Title"},
        {"FEEL SO CLOSE", "Song Title"},
        {"HEADLINES", "Song Title"},
        {"DIE YOUNG", "Song Title"},
        {"AM I WRONG", "Song Title"},
        {"RHYTHM AS A DANCER", "Song Title"},
        {"TELEPHONE", "Song Title"},
        {"UNFORGETTABLE", "Song Title"},
        {"LEAN ON ME", "Song Title"},
        {"LEAN ON", "Song Title"},

        // COUNTRIES
        {"ITALY", "Country"},
        {"POLAND", "Country"},
        {"SOUTH KOREA", "Country"},
        {"GREECE", "Country"},
        {"SWITZERLAND", "Country"},
        {"AUSTRALIA", "Country"},

        // PLACE
        {"AMUSEMENT PARK", "Place"},
        {"THE ZOO", "Place"},
        {"THE BEACH", "Place"},
        {"MOVIE THEATER", "Place"},
        {"COFFEE SHOP", "Place"},
        {"WESTERN UNIVERSITY", "Place"},
        {"TRAMPOLINE PARK", "Place"},

        // QUOTATION
        {"PRACTICE MAKES PERFECT", "Quotation"},
        {"BETTER LATE THAN NEVER", "Quotation"},
        {"KNOWLEDGE IS POWER", "Quotation"},
        {"YOU ONLY LIVE ONCE", "Quotation"},
        {"HOME SWEET HOME", "Quotation"},
        {"NEVER SAY NEVER", "Quotation"},
        {"THE EARLY BIRD GETS THE WORM", "Quotation"},
        {"OUT OF SIGHT OUT OF MIND", "Quotation"},

        // THING
        {"RED BALLOON", "Thing"},
        {"WATER BOTTLE", "Thing"},
        {"TEDDY BEAR", "Thing"},
        {"SUNGLASSES", "Thing"},
        {"COMPUTER MOUSE", "Thing"},
        {"PAINTBRUSH", "Thing"},
        {"UMBRELLA", "Thing"},
        {"SKATEBOARD", "Thing"},
        {"HEADPHONES", "Thing"},
        {"CANDLE", "Thing"},

        // ARTISTS
        {"JUSTIN BIEBER", "Artist"},
        {"TAYLOR SWIFT", "Artist"},
        {"KATY PERRY", "Artist"},
        {"SELENA GOMEZ", "Artist"},
        {"ARIANA GRANDE", "Artist"},
        {"BRUNO MARS", "Artist"},
        {"ED SHEERAN", "Artist"},
    };

    // HARD PHRASES
    hardPhrase = {
        // FOOD
        {"PAPIYA", "Food"},
        {"POMMAGRANTE", "Food"},
        {"ELDERBERRY", "Food"},
        {"CHICKEN SOUVLAKI", "Food"},
        {"BRUSCHETTA", "Food"},
        {"CHARCUTERIE", "Food"},
        {"GNOCCHI", "Food"},
        {"ESCARGOT", "Food"},
        {"MASCARPONE CHEESE", "Food"},
        {"BUCKWHEAT", "Food"},
        {"TIRAMISU", "Food"},

        // ANIMAL
        {"HIPPOPOTAMUS", "Animal"},
        {"CATERPILLAR", "Animal"},
        {"ORANGUTAN", "Animal"},
        {"CHIMPANZEE", "Animal"},
        {"RHINOCEROS", "Animal"},
        {"TASMANIAN DEVIL", "Animal"},
        {"TARANTULA", "Animal"},

        // MOVIE TITLE
        {"PIRATES OF THE CARIBBEAN", "Movie Title"},
        {"THE LORD OF THE RINGS", "Movie Title"},
        {"THE CHRONICLES OF NARNIA", "Movie Title"},
        {"A STREETCAR NAMED DESIRE", "Movie Title"},
        {"HOW TO TRAIN YOUR DRAGON", "Movie Title"},
        {"THE SECRET LIFE OF PETS", "Movie Title"},
        {"SPIDERMAN INTO THE SPIDERVERSE", "Movie Title"},

        // SONG TITLE
        {"SMELLS LIKE TEEN SPIRIT", "Song Title"},
        {"VIVA LA VIDA", "Song Title"},
        {"STAIRWAY TO HEAVEN", "Song Title"},
        {"ANOTHER ONE BITES THE DUST", "Song Title"},
        {"BOHEMIAN RHAPSODY", "Song Title"},
        {"SINCE U BEEN GONE", "Song Title"},
        {"SOMEBODY THAT I USED TO KNOW", "Song Title"},
        {"WHAT A WONDERFUL WORLD", "Song Title"},
        {"LA DANZA", "Song Title"},
        {"WATCH THE SUNRISE", "Song Title"},
        {"HOW TO SAVE A LIFE", "Song Title"},
        {"POCKETFUL OF SUNSHINE", "Song Title"},
        {"SWEET DEPOSITION", "Song Title"},
        {"GUATEMALA", "Song Title"},

        // COUNTRIES
        {"BOSNIA AND HERZEGOVINA", "Country"},
        {"CZECH REPUBLIC", "Country"},
        {"UNITED ARAB EMIRATES", "Country"},
        {"TRINIDAD AND TOBAGO", "Country"},
        {"NORTH MACEDONIA", "Country"},
        {"EL SALVADOR", "Country"},
        {"DEMOCRATIC REPUBLIC OF THE CONGO", "Country"},
        {"VATICAN CITY", "Country"},
        {"LIECHTENSTEIN", "Country"},

        // PLACE
        {"MOUNT EVEREST", "Place"},
        {"THE SAHARA DESERT", "Place"},
        {"EIFFEL TOWER", "Place"},
        {"NIAGARA FALLS", "Place"},
        {"THE GREAT WALL OF CHINA", "Place"},
        {"AMAZON RAINFOREST", "Place"},
        {"MOUNT KILIMANJARO", "Place"},
        {"MIDDLESEX COLLEGE", "Place"},

        // QUOTATION
        {"TO BE OR NOT TO BE THAT IS THE QUESTION", "Quotation"},
        {"I THINK THEREFORE I AM", "Quotation"},

        // THING
        {"ELECTRIC GUITAR", "Thing"},
        {"VINTAGE CAMERA", "Thing"},
        {"SEWING MACHINE", "Thing"},
        {"VINYL RECORD", "Thing"},
        {"BINOCULARS", "Thing"},
        {"HARMONICA", "Thing"},

        // ARTISTS
        {"MICHAEL JACKSON", "Artist"},
        {"ELVIS PRESLEY", "Artist"},
        {"WHITNEY HOUSTON", "Artist"},
        {"CELINE DION", "Artist"},
        {"JOHN SUMMIT", "Artist"},
        {"ODD MOB", "Artist"},
        {"AYYBO", "Artist"},
        {"DOM DOLLA", "Artist"},
        {"CHRIS LAKE", "Artist"},
    };

    lastEasyIndex = -1; // to track last used easy phrase
    lastHardIndex = -1; // to track last used hard phrase
}

Phrase PhraseLibrary::getRandomPhrase(const std::string &difficulty) {
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
        return {"No Phrases Available", "None"};
    }
}
