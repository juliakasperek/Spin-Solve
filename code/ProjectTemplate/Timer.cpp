#include "Timer.h"

Timer::Timer() {

}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
}

double Timer::getSeconds() const {
    std::chrono::duration<double> elapsed = endTime - startTime;

    return elapsed.count();
}
