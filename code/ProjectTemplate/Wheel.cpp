#include "Wheel.h"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>

Wheel::Wheel() {
    segments = {
        "minus 5 seconds",
        "minus 5 seconds",
        "5 gems",
        "2 gems",
        "2 gems",
        "2 gems",
        "2 gems",
        "free hint"
    };

    std::srand(std::time(nullptr));
}

std::string Wheel::spin() {
    int spinLength = 20 + std::rand() % 10; // number of steps in the spin
    int index = 0;

    std::cout << "Spinning the wheel...\n";

    for (int i = 0; i < spinLength; i++) {
        // clear line and print current segment
        std::cout << "\r" << segments[index] << "   ";
        std::cout.flush();

        // increase delay to simulate slowing down
        std::this_thread::sleep_for(std::chrono::milliseconds(50 + i * 20));

        // move to next segment
        index = (index + 1) % segments.size();
    }

    std::cout << "\nThe wheel landed on: " << segments[index] << "!\n";
    return segments[index];
}

